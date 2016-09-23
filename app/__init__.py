from flask import Flask, render_template, redirect
import time
import os
def current_milli_time():
	return int(round(time.time()))

app = Flask(__name__)
board = [0]*225
wait = 0
variation = []
evaluation = 0
from sandbox import get_sandbox

cwd = os.getcwd()
bot = get_sandbox(cwd)
@app.route('/')
@app.route('/home/')
def home():
	global board
	global wait
	global evaluation
	global variation
	return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation)

@app.route('/var', methods=['GET'])
def variation():
	global variation
	global board
	global wait
	global evaluation
	for i, v in enumerate(variation):
		board[v] = i + 3
	wait = 2
	return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation)

@app.route('/unvar', methods=['GET'])
def unvariation():
	global variation
	global board
	global wait
	global evaluation
	for i, v in enumerate(variation):
		board[v] = 0
	wait = 1
	return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation)


@app.route('/start/<int:start>', methods=['POST', 'GET'])
def start_game(start):
	global bot
	global variation
	global board
	global wait
	global evaluation
	print 'hi'
	bot.start('./AI')
	print 'hi'
	wait = 1
	if start == 1:
		board[225//2] = 2
	bot.write_line(str(start))
	return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation)

@app.route('/play/<int:move>', methods=['POST', 'GET'])
def play_move(move):
	global board
	global evaluation
	global variation
	global bot
	global wait
	bot.write_line(str(move))
	board[move] = 1
	w = None
	i = 0
	t = current_milli_time()
	while w is None or 'n' not in w:
		i += 1
		w = bot.read_line()
		if i % 100 == 0:
			if current_milli_time() - t > 15:
				return redirect('/end', code='400')
				break;
	lst = map(lambda x: int(x), w.split(' ')[:-1])
	evaluation = lst[1]
	move = lst[0]
	variation = lst[3:]
	print move, evaluation, variation
	board[move] = 2
	return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation)

@app.route('/end', methods=['POST', 'GET'])
def end_game():
	global bot
	global variation
	global board
	global wait
	global evaluation
	bot.kill()
	wait = 0
	board = [0]*225
	return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation)

if __name__ == '__main__':
	app.run(port='28000');