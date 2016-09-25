from flask import Flask, render_template, redirect
import time
import os
def current_milli_time():
	return int(round(time.time()))
wl = open('wl', 'r').read().split(' ')
app = Flask(__name__)
board = [0]*225
wait = 0
options = [0, 0, 0, int(wl[0]), int(wl[1])] # started? variation_mode?
variation = []
evaluation = 0
from sandbox import get_sandbox

def increment(w, l):
	global options
	options[3] += w
	options[4] += l
	open('wl', 'w').write(str(options[3]) + ' ' + str(options[4]))

cwd = os.getcwd()
bot = get_sandbox(cwd)
@app.route('/')
@app.route('/home/')
def home():
	global board
	global wait
	global options
	global evaluation
	global variation
	return render_template('home.html', options=options, wait=wait, board=board, evaluation=evaluation, variation=variation)

@app.route('/var', methods=['GET'])
def variation():
	global variation
	global board
	global wait
	global options
	global evaluation
	for i, v in enumerate(variation):
		board[v] = i + 3
	wait = 2
	options[1] = 1
	return render_template('home.html', wait=wait, options=options, board=board, evaluation=evaluation, variation=variation)

@app.route('/unvar', methods=['GET'])
def unvariation():
	global variation
	global board
	global wait
	global options
	global evaluation
	global options
	for i, v in enumerate(variation):
		board[v] = 0
	wait = 1
	options[1] = 0
	return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation, options=options)


@app.route('/start/<int:start>', methods=['POST', 'GET'])
def start_game(start):
	global bot
	global variation
	global board
	global wait
	global options
	global evaluation
	print 'hi'
	bot.start('./AI')
	print 'hi'
	options[0] = 1
	wait = 1
	if start == 1:
		board[225//2] = 2
	bot.write_line(str(start))
	return render_template('home.html', wait=wait, board=board, evaluation=evaluation, options=options, variation=variation)

@app.route('/play/<int:move>', methods=['POST', 'GET'])
def play_move(move):
	global board
	global evaluation
	global variation
	global bot
	global options
	global wait
	data = open('./static/possibilities.txt').read()
	array = data.split("\n")
	board[move] = 1
	for arr in array:
		ar = arr.split(" ")
		if (board[int(ar[0])] == 1 and board[int(ar[1])] == 1 and board[(int(ar[2]))] == 1 and board[(int(ar[3]))] == 1 and board[(int(ar[4]))] == 1):
			options[2] = 1
			wait = 10
			increment(1, 0)
			return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation, options=options)
	bot.write_line(str(move))
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
	for arr in array:
		ar = arr.split(" ")
		if (board[int(ar[0])] == 2 and board[int(ar[1])] == 2 and board[(int(ar[2]))] == 2 and board[(int(ar[3]))] == 2 and board[(int(ar[4]))] == 2):
			options[2] = 2
			wait = 10
			increment(0, 1)
			return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation, options=options)
	return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation, options=options)

@app.route('/end', methods=['POST', 'GET'])
def end_game():
	global bot
	global variation
	global board
	global wait
	global options
	global evaluation
	bot.kill()
	options[0] = 0
	options[1] = 0
	options[2] = 0
	wait = 0
	board = [0]*225
	return render_template('home.html', wait=wait, board=board, evaluation=evaluation, variation=variation, options=options)

if __name__ == '__main__':
	app.run(host='0.0.0.0', port=28001);
