import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    if request.method == "POST":
        user_balance = float(db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])[0]["cash"])
        funded_money = request.form.get("fund")

        if not funded_money:
            return apology("Must provide amount")
        else:
            try:
                if float(funded_money) <= 0:
                    return apology("Fund Money must be positive")
                db.execute("UPDATE users SET cash = (?) WHERE id = (?)", user_balance + float(funded_money), session["user_id"])
                return redirect("/")
            except ValueError:
                return apology("Shares must be numerical")
    else:
        table = db.execute("""
            SELECT T1.stock_symbol, T1.total_sum - COALESCE(T2.total_sum, 0) AS total_shares
            FROM (SELECT stock_symbol, SUM(shares) AS total_sum FROM purchase WHERE user_id = (?) GROUP BY stock_symbol) AS T1
            LEFT JOIN
            (SELECT stock_symbol, SUM(shares) AS total_sum FROM selled WHERE user_id = (?) GROUP BY stock_symbol) AS T2
            ON T1.stock_symbol = t2.stock_symbol""", session["user_id"], session["user_id"])

        total_stock_value = 0
        for row in table:
            row["price"] = float(lookup(row["stock_symbol"])["price"])
            row["stock_value"] = row["total_shares"] * row["price"]
            total_stock_value += row["stock_value"]

        cash = float(db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])[0]["cash"])
        total_value = cash + total_stock_value
        return render_template("index.html", stock=table, cash=cash, total_value=total_value)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "GET":
        return render_template("buy.html")
    else:
        Symbol = request.form.get("symbol")
        Shares = request.form.get("shares")

        if not Symbol:
            return apology("Must provide Symbol")
        elif not lookup(Symbol):
            return apology("Symbol doesn't exist")
        elif not Shares:
            return apology("Must provide Shares")
        elif not Shares.isdigit() or int(Shares) <= 0:
            return apology("Shares must be numerical")
        else:
            user_balance = float(db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])[0]["cash"])
            price = float(lookup(Symbol)["price"])
            total_price = price * int(Shares)
            if user_balance < price:
                return apology("Insufficent balance")
            else:
                db.execute("UPDATE users SET cash = (?) WHERE id = (?)", user_balance - total_price, session["user_id"])
                db.execute("INSERT INTO purchase (user_id, stock_symbol, shares, price, time) VALUES(?, ?, ?, ?, CURRENT_TIMESTAMP)", session["user_id"], Symbol, Shares, price)
                return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    table = db.execute("SELECT * FROM purchase UNION ALL SELECT * FROM selled ORDER BY time")
    for row in table:
        if row["transaction_type"] == "Selled":
            row["shares"] = -1 * int(row["shares"])
    return render_template("history.html", table=table)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "GET":
        return render_template("quote.html")
    else:
        Symbol = request.form.get("symbol")

        if not Symbol:
            return apology("Must provide Symbol.")
        elif not lookup(Symbol):
            return apology("Symbol doesn't exist.")
        else:
            return render_template("quoted.html", symbol=lookup(Symbol))

@app.route("/register", methods=["GET", "POST"])
def register():
    """ Register User"""

    if request.method == "GET":
        return render_template("register.html")
    else:
        username = request.form.get("username")
        password = request.form.get("password")
        confirm_password = request.form.get("confirmation")

        #check if the user provided username and password
        if not username:
            return apology("Must provide Username.")
        elif not password:
            return apology("Must provide Password.")

        #check if the passwords match
        elif not confirm_password:
            return apology("Must Confirm Password.")
        elif password != confirm_password:
            return apology("Password must match.")
        elif True:

            #checks if the username is already taken
            try:
                db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))

                #creates a session for the user
                session["user_id"] = db.execute("SELECT id FROM users WHERE username = (?) ", username)[0]["id"]
                return redirect("/")
            except ValueError:
                return apology("Username Taken!")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "GET":
        table = db.execute("""
        SELECT T1.stock_symbol, T1.total_sum - COALESCE(T2.total_sum, 0) AS total_shares
        FROM (SELECT stock_symbol, SUM(shares) AS total_sum FROM purchase WHERE user_id = (?) GROUP BY stock_symbol) AS T1
        LEFT JOIN
        (SELECT stock_symbol, SUM(shares) AS total_sum FROM selled WHERE user_id = (?) GROUP BY stock_symbol) AS T2
        ON T1.stock_symbol = t2.stock_symbol""", session["user_id"], session["user_id"])

        return render_template("sell.html", symbols=table)
    else:
        Symbol = request.form.get("symbol")
        Shares = request.form.get("shares")
        rows = db.execute("""
        SELECT T1.stock_symbol, T1.total_sum - COALESCE(T2.total_sum, 0) AS total_shares
        FROM (SELECT stock_symbol, SUM(shares) AS total_sum FROM purchase WHERE user_id = (?) GROUP BY stock_symbol) AS T1
        LEFT JOIN
        (SELECT stock_symbol, SUM(shares) AS total_sum FROM selled WHERE user_id = (?) GROUP BY stock_symbol) AS T2
        ON T1.stock_symbol = t2.stock_symbol WHERE T1.stock_symbol = (?)""", session["user_id"], session["user_id"], Symbol)


        if not Symbol:
            return apology("Must provide Symbol")
        elif not lookup(Symbol):
            return apology("Symbol doesn't exist")
        elif not rows:
            return apology("You don't own stock from this symbol")
        elif not Shares:
            return apology("Must provide Shares")
        elif not Shares.isdigit() or int(Shares) <= 0:
            return apology("Shares must be numerical")
        elif int(Shares) > rows[0]["total_shares"]:
            return apology("Shares is more than you own")
        else:
            user_balance = int(db.execute("SELECT cash FROM users WHERE id = (?)", session["user_id"])[0]["cash"])
            price = float(lookup(Symbol)["price"])
            total_price = price * int(Shares)

            db.execute("UPDATE users SET cash = (?) WHERE id = (?)", user_balance + total_price, session["user_id"])
            db.execute("INSERT INTO selled (user_id, stock_symbol, shares, price, time) VALUES(?, ?, ?, ?, CURRENT_TIMESTAMP)", session["user_id"], Symbol, Shares, price)
            return redirect("/")




