from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
@app.route('/home/')

def home():
    user = {'nickname': 'Miguel'}  # fake user
    return render_template('home.html', title="HomePage Random", user=user)


if __name__ == '__main__':
    app.run()