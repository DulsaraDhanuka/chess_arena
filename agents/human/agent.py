import chess

class HumanAgent():
    def __init__(self, name):
        self.name = name

    def next_move(self, board: chess.Board, last_move: str, next_legal_moves: chess.LegalMoveGenerator):
        print(board)
        print()
        print([move.uci() for move in next_legal_moves])
        return input(f"{self.name} > ")

    def reset(self, self_color):
        pass

    def terminate(self):
        pass
