import random
import chess
import chess.svg
from agents.human.agent import HumanAgent
from agents.random.agent import RandomAgent
from agents.stockfish.agent import StockfishAgent

board = chess.Board()

agent1 = RandomAgent("Human")
agent2 = StockfishAgent("Stockfish")
players = [agent1, agent2]

current_player = random.randint(0, 1)
white_player = current_player
last_move = None
print(f"White: {players[current_player].name}, Black: {players[int(not current_player)].name}")
while not board.is_game_over():
    try:
        move = players[current_player].next_move(board, last_move, board.legal_moves)
        board.push_uci(move)
        current_player = int(not current_player)
        last_move = move
    except chess.IllegalMoveError as e:
        print("Illegal move")
        continue

result = board.outcome()
if result.winner is None:
    print("Draw")
    print(result)
elif result.winner is True:
    print(f"{players[white_player].name} Wins!")
else:
    print(f"{players[int(not white_player)].name} Wins!")
