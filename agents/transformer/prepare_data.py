import os
import chess
import chess.pgn
from data_utils import Encoding
import numpy as np

dir_path = os.path.dirname(os.path.realpath(__file__))
data_path = os.path.join(dir_path, "data")

pgn_files = [f for f in os.listdir(data_path) if os.path.isfile(os.path.join(data_path, f)) and os.path.splitext(f)[1] == ".pgn"]
enc = Encoding()
tokens = []
for pgn_name in pgn_files:
    with open(os.path.join(data_path, pgn_name), 'r', encoding='utf-8') as pgn:
        while True:
            try:
                game = chess.pgn.read_game(pgn)
                if game is None: break
                tokens.append(enc.encode("<s>"))
                for move in game.mainline_moves():
                    tokens.append(enc.encode(move.uci()))
                tokens.append(enc.encode("</s>"))
            except Exception as e:
                continue
n_vocab = enc.n_vocab
print(f"Training data loaded - {len(tokens)} tokens")

with open("data.npy", "wb") as f:
    np.save(f, np.array(tokens, dtype=np.int32))
