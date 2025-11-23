import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from functools import wraps

# Configure application
app = Flask(__name__)



# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finalproject.db")

libraries = ["ALL", "OWNFOODS", "FAVOURITES"]

def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/latest/patterns/viewdecorators/
    """

    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)

    return decorated_function


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
    libraries = ["ALL", "OWNFOODS", "FAVOURITES"]

    if request.method == "GET":
        list_of_foods = db.execute("SELECT * FROM ownFood ORDER BY title ASC")
        return render_template("homepage.html", list_of_foods=list_of_foods, libraries=libraries)
    else:
        selected_library = request.form.get("library")
        search = request.form.get("search")
        order = request.form.get("order")

        if order:
            if not selected_library and search:
                list_of_foods = db.execute("SELECT * FROM ownFood WHERE title LIKE (% ? %) ORDER BY (?) DESC", search)
            if selected_library and not search:
                list_of_foods = db.execute("SELECT * FROM ownFood WHERE id IN (SELECT * FROM {selected_library}) ORDER BY (?) DESC", order)
            if selected_library and search:
                list_of_foods = db.execute("SELECT * FROM ownFood WHERE id IN (SELECT * FROM {selected_library}) AND title LIKE (% ? %) ORDER BY (?) DESC", search, order)
        else:
            if not selected_library and search:
                list_of_foods = db.execute("SELECT * FROM ownFood WHERE title LIKE (% ? %) ORDER BY title ASC", search)
            if selected_library and not search:
                list_of_foods = db.execute("SELECT * FROM ownFood WHERE id IN (SELECT * FROM {selected_library}) AND user_id = (?) ORDER BY title ASC")
            if selected_library and search:
                list_of_foods = db.execute("SELECT * FROM ownFood WHERE id IN (SELECT * FROM {selected_library}) AND title LIKE (% ? %) ORDER BY title ASC", search)

        return render_template("homepage.html", list_of_foods=list_of_foods, libraries=libraries)

@app.route("/addlibrary")
@login_required
def addlibrary():

    table = request.form.get("table")
    food = request.form.get("food_id")

    db.execute = ("INSERT INTO libraries (user_id, library, food_id) VALUES ( ?, ?, ?)", session["user_id"], table, food)

    library.append(table)
    return redirect("/")


@app.route("/newlibrary")
@login_required
def newlibrary():
    return redirect("/")




@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")

    # Get form fields
    name = request.form.get("name")
    email = request.form.get("email")
    password = request.form.get("password")

    # Validate empty fields
    if not (name and email and password):
        flash("Input fields must not be empty")
        return redirect("/register")

    # Attempt to insert user
    try:
        db.execute(
            "INSERT INTO users (email, name, hash) VALUES(?, ?, ?)",
            email, name, generate_password_hash(password))

        # Create session
        session["user_id"] = db.execute(
            "SELECT id FROM users WHERE email = ?", email)[0]["id"]

        flash("Registration successful! Please log in.")
        return redirect("/login")

    except ValueError:
        flash("Email already exists")
        return redirect("/register")


@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "GET":
        session.clear()
        return render_template("login.html")
    else:
        # Get form fields
        email = request.form.get("email")
        password = request.form.get("password")

        # Validate empty fields
        if not (email and password):
            flash("Input fields must not be empty")
            return redirect("/login")

        # check if the user exist and password is correct
        user = db.execute("SELECT * FROM users WHERE email=(?)", email)

        if len(user) == 1 and check_password_hash(user[0]["hash"], password):
            session["user_id"] = user[0]["id"]
            return redirect("/")


@app.route("/food")
@login_required
def food():
    return render_template("food.html")
