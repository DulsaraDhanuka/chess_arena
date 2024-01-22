import chess
import dotenv
from stockfish import Stockfish

class StockfishAgent():
    def __init__(self, name):
        self.name = name
        self.config = dotenv.dotenv_values(".env")
        self.stockfish = Stockfish(path=self.config['STOCKFISH_PATH'])

    def next_move(self, board: chess.Board, last_move: str, next_legal_moves: chess.LegalMoveGenerator):
        if last_move is not None:
            self.stockfish.make_moves_from_current_position([last_move])
        move = self.stockfish.get_best_move()
        self.stockfish.make_moves_from_current_position([move])
        return move
