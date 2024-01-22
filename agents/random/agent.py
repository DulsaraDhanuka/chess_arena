import chess
import random

class RandomAgent():
    def __init__(self, name):
        self.name = name

    def next_move(self, board: chess.Board, last_move: str, next_legal_moves: chess.LegalMoveGenerator):
        legal_moves = [move.uci() for move in next_legal_moves]
        return random.choice(legal_moves)
