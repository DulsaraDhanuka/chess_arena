import chess
import dotenv
from stockfish import Stockfish

class StockfishAgent():
    def __init__(self, name):
        self.name = name
        self.config = dotenv.dotenv_values(".env")
        params = {
                "Debug Log File": "",
                "Contempt": 0,
                "Min Split Depth": 0,
                "Threads": 2, # More threads will make the engine stronger, but should be kept at less than the number of logical processors on your computer.
                "Ponder": "false",
                "Hash": 2048, # Default size is 16 MB. It's recommended that you increase this value, but keep it as some power of 2. E.g., if you're fine using 2 GB of RAM, set Hash to 2048 (11th power of 2).
                "MultiPV": 1,
                "Skill Level": 20,
                "Move Overhead": 10,
                "Minimum Thinking Time": 20,
                "Slow Mover": 100,
                "UCI_Chess960": "false",
                "UCI_LimitStrength": "false",
            }
        self.stockfish = Stockfish(path=self.config['STOCKFISH_PATH'], depth=35)
        self.default_fen = chess.Board().fen()
        self.stockfish.set_fen_position(self.default_fen)


    def next_move(self, board: chess.Board, last_move: str, next_legal_moves: chess.LegalMoveGenerator):
        self.stockfish.set_fen_position(board.fen())
        move = self.stockfish.get_best_move()
        return move

    def reset(self):
        self.stockfish.set_fen_position(self.default_fen)

    def terminate(self):
        pass
