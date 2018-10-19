import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

#################################################
@app.route("/")
@login_required
def index():

    return apology("INDEX: in progress")
"""
    # Get owned stocks
    portfolio = db.execute("SELECT shares, symbol \
                                    FROM portfolio WHERE id = :id", \
                                    id=session["user_id"])

    if not portfolio:
        return apology("INDEX: SELECT failed")

    # create a temporary variable to store TOTAL worth ( cash + share)
    total_cash = 0

    # update each symbol prices and total
    for portfolio_symbol in portfolio_symbols:
        symbol = portfolio_symbol["symbol"]
        shares = portfolio_symbol["shares"]
        stock = lookup(symbol)
        total = shares * stock["price"]
        total_cash += total
        db.execute("UPDATE portfolio SET price=:price, \
                    total=:total WHERE id=:id AND symbol=:symbol", \
                    price=usd(stock["price"]), \
                    total=usd(total), id=session["user_id"], symbol=symbol)

    # update user's cash in portfolio
    updated_cash = db.execute("SELECT cash FROM users \
                               WHERE id=:id", id=session["user_id"])

    # update total cash -> cash + shares worth
    total_cash += updated_cash[0]["cash"]

    # print portfolio in index homepage
    updated_portfolio = db.execute("SELECT * from portfolio \
                                    WHERE id=:id", id=session["user_id"])

    return render_template("index.html", stocks=updated_portfolio, \
                            cash=usd(updated_cash[0]["cash"]), total= usd(total_cash) )
"""


#################################################
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        return apology("TODO")

#################################################
@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    return apology("TODO")

#################################################
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method != "POST":
        return render_template("login.html")
    # User reached route via POST (as by submitting a form via POST)
    else:
        # Ensure username was submitted
        # DJM any reason I can do the form get into a variable here and reuse it later?
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must also provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Save logged in
        session["user_id"] = rows[0]["id"]

        # Send back to home with auth after logging in
        return redirect("/")


#################################################
@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

#################################################
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("Must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("Must provide password")

        # ensure password and verified password is the same
        elif request.form.get("password") != request.form.get("passwordchk"):
            return apology("password doesn't match")

        # insert the new user into users, storing the hash of the user's password
        res = db.execute("INSERT INTO users (username, hash) \
                             VALUES(:username, :hash)", \
                             username=request.form.get("username"), \
                             hash=generate_password_hash(request.form.get("password")))

        if not res:
            return apology("INSERT failed. User may already exist?", 401)

        # remember which user has logged in
        session["user_id"] = res

        # redirect user to home page
        return redirect("/")

    else:
        return render_template("register.html")
"""
    # Forget any user_id
    session.clear()

    # User reached route via GET (as by clicking a link or via redirect)
    if request.method != "POST":
        return render_template("register.html")
    # User reached route via POST (as by submitting a form via POST)
    else:
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("New user must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("New user must also provide password", 403)

        # Get 2nd pwd for verify
        elif not request.form.get("passwordchk"):
            return apology("New user must also repeat password", 403)

        pw1=request.form.get("password")
        pw2=request.form.get("passwordchk")

        if pw1 != pw2:
            return apology("Passwords must be the same, try again", 403)

        # Write to DB with hashed pwd
        rows = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",\
        username=request.form.get("username"),\
        hash=generate_password_hash(request.form.get("password")))

        if not rows:
            return apology("INSERT failed. User may already exist?", 401)

        # Remember which user has logged in
        user_id = db.execute("SELECT id FROM users WHERE username IS :username",\
                            username=request.form.get("username"))
        session["user_id"] = rows

        # Redirect user to home page
        return redirect("/")
"""

#################################################
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method != "POST":
        return render_template("quote.html")
    else:
        mystock = lookup(request.form.get("sym"))

        if not mystock:
            return apology("$s - symbol unknown", mystock)

        mystock['price'] = usd(mystock['price'])
        return render_template("quote.html", stock=mystock)


#################################################
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    return apology("TODO")

