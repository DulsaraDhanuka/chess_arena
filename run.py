import random
import chess
import chess.svg
from agents.human.agent import HumanAgent
from agents.random.agent import RandomAgent
from agents.stockfish.agent import StockfishAgent
from agents.transformer.agent import TransformerAgent

def run_game():
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

board = chess.Board()

agent1 = RandomAgent("Dummy")
#agent1 = StockfishAgent("Stockfish")
#agent1 = HumanAgent("Human")
agent2 = TransformerAgent("Transformer")
players = [agent1, agent2]

wins = {}
total_matches = 0
for _ in range(1):
    for player in players:
        player.reset()
    current_player = random.choice(players)
    white_player = current_player
    black_player = players[int(not players.index(current_player))]
    last_move = None
    print(f"White: {white_player.name}, Black: {black_player.name}")
    while not board.is_game_over():
        try:
            move = current_player.next_move(board, last_move, board.legal_moves)
            board.push_uci(move)
            current_player = white_player if current_player == black_player else black_player
            last_move = move
        except chess.IllegalMoveError as e:
            print("Illegal move")
            continue

    result = board.outcome()
    if result.winner is None:
        print("Draw")
        print(result)
    elif result.winner is True:
        print(f"{white_player.name} Wins!")
        wins[white_player.name] = wins.get(white_player.name, 0) + 1
    else:
        print(f"{black_player.name} Wins!")
        wins[black_player.name] = wins.get(black_player.name, 0) + 1
    total_matches += 1

print("\n")
print(f"Total matches: {total_matches}")
for name, total in wins.items():
    print(f"{name}: {total}")

for player in players:
    player.terminate()
