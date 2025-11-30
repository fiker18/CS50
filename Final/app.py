import os

from werkzeug.utils import secure_filename
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from functools import wraps

# Configure application
app = Flask(__name__)

UPLOAD_FOLDER = "static/uploads"


# Configure session to use filesystem (instead of signed cookies)

app.config["UPLOAD_FOLDER"] = UPLOAD_FOLDER
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finalproject.db")



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



@app.route("/")
@login_required
def index():
    libraries = []

    lib = db.execute("SELECT library FROM library WHERE user_id = (?) GROUP BY library", session["user_id"])
    for dic in lib:
        libraries.append(dic["library"])

    selected_library = request.args.get("library")
    search = request.args.get("search")

    if selected_library:
        list_of_foods = db.execute("SELECT * FROM ownFood WHERE foodid IN (SELECT food_id FROM library WHERE user_id = (?) AND library = (?)) ORDER BY TITLE", session["user_id"], selected_library)

        if not list_of_foods or selected_library == "ALL":
            list_of_foods = db.execute("SELECT * FROM ownFood ORDER BY TITLE")
    elif search:
        list_of_foods = db.execute("SELECT * FROM ownFood WHERE TITLE LIKE (?) ORDER BY TITLE", search)
        selected_library = "ALL"
    else:
        list_of_foods = db.execute("SELECT * FROM ownFood ORDER BY TITLE")
        selected_library = "ALL"

    return render_template("homepage.html", list_of_foods=list_of_foods, libraries=libraries, selected_library=selected_library)



@app.route("/set_library", methods=["POST"])
@login_required
def newlibrary():
    library = request.form.get("library")
    new_library = request.form.get("new_library")
    food_id = request.form.get("food_id")

    if (library) and (food_id):
        db.execute("INSERT INTO library (user_id, food_id, library) VALUES ( ?, ?, ?)", session["user_id"], food_id, library)
    elif (new_library) and (food_id):
        db.execute("INSERT INTO library (user_id, food_id, library) VALUES ( ?, ?, ?)", session["user_id"], food_id, new_library)
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
        else:
            flash("Username or Password Incorrect")
            return redirect("/login")


@app.route("/food")
@login_required
def food():
    libraries = []

    lib = db.execute("SELECT library FROM library WHERE user_id = (?) GROUP BY library", session["user_id"])
    for dic in lib:
        libraries.append(dic["library"])

    food_id = request.args.get("food_id")
    if not food_id:
        return render_template("food.html")
    else:
        food = db.execute("SELECT * FROM ownFood WHERE foodid = (?)", food_id)
        return render_template("food.html", food=food[0], libraries=libraries)


@app.route("/foodcreate", methods=["GET", "POST"])
@login_required
def save_food():
    if request.method == "GET":
         return render_template("foodcreate.html")
    else:
        title = request.form.get("title")
        description = request.form.get("description")
        ingredients = request.form.get("ingredients")
        procedure = request.form.get("procedure")
        image = request.files.get("image")
        duration = request.form.get("duration")

        # Validate fields
        if not (title and description and ingredients and procedure):
            flash("All fields are required")
            return redirect("/foodcreate")

        # Handle image upload
        image_filename = None
        if image and image.filename != "":
            image_filename = secure_filename(image.filename)
            image.save(os.path.join(app.config["UPLOAD_FOLDER"], image_filename))

        # Insert into database
        db.execute(
            """
            INSERT INTO ownFood (TITLE, DESCRIPTION, RECIPE, PROCEDURE, DURATIONS, IMAGE, user_id)
            VALUES (?, ?, ?, ?, ?, ?, ?)
            """,
            title,
            description,
            ingredients,
            procedure,
            duration,
            image_filename,
            session["user_id"]
        )
        return redirect("/")
