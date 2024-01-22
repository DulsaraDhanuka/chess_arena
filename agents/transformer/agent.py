import chess

class TransformerAgent():
    def __init__(self, name):
        self.name = name

    def next_move(self, board: chess.Board, last_move: str, next_legal_moves: chess.LegalMoveGenerator):
        legal_moves = [move.uci() for move in next_legal_moves]
        return legal_moves[0]
