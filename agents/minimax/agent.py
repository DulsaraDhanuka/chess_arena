
import chess

class MinimaxAgent():
    def __init__(self, name):
        self.name = name

    def calculate_score(self, board: chess.Board) -> int:
        invert = -1 if board.turn is chess.BLACK else 1
        score = 0
        for _, piece in board.piece_map().items():
            if piece.piece_type is chess.PAWN:
                score += 10 * invert * (-1 if piece.color is chess.BLACK else 1)
            elif piece.piece_type is chess.KNIGHT:
                score += 30 * invert  * (-1 if piece.color is chess.BLACK else 1)
            elif piece.piece_type is chess.BISHOP:
                score += 30 * invert * (-1 if piece.color is chess.BLACK else 1)
            elif piece.piece_type is chess.ROOK:
                score += 50 * invert * (-1 if piece.color is chess.BLACK else 1)
            elif piece.piece_type is chess.QUEEN:
                score += 90 * invert * (-1 if piece.color is chess.BLACK else 1)
        if board.is_check():
            score -= 900
        if board.is_checkmate():
            score -= 9999
        return score
    def execute_depth_unit(self, board, graph, depth):
        for move in board.legal_moves:
            board.push_uci(move.uci())
            graph[move.uci()] = {}
            for altr_move in board.legal_moves:
                board.push_uci(altr_move.uci())
                score = self.calculate_score(board)
                if depth > 1:
                    graph[move.uci()][altr_move.uci()] = {}
                    self.execute_depth_unit(board, graph[move.uci()][altr_move.uci()], depth-1)
                else:
                    graph[move.uci()][altr_move.uci()] = score
                board.pop()
            board.pop()
    def construct_graph(self, board, depth):
        graph = {}
        new_board = chess.Board(board.fen())
        self.execute_depth_unit(new_board, graph, depth)
        return graph
    def minimax(self, graph):
        minimax_graph = {}
        for move, node in graph.items():
            min_score = float('inf')
            for white_move, sub_node in node.items():
                if isinstance(sub_node, dict):
                    _, score = self.minimax(sub_node)
                else:
                    score = sub_node
                if score < min_score:
                    min_score = score
            minimax_graph[move] = min_score
        moves = list(minimax_graph.keys())
        scores = list(minimax_graph.values())
        return moves[scores.index(max(scores))], max(scores)
    def alpha_beta_max(self, board, alpha, beta, depth_left):
        if (depth_left == 0): return None, self.calculate_score(board)
        selected_move = None
        for move in board.legal_moves:
            board.push_uci(move.uci())
            _, score = self.alpha_beta_min(board, alpha, beta, depth_left-1)
            board.pop()
            if score >= beta:
                return move.uci(), beta
            if score > alpha:
                alpha = score
                selected_move = move.uci()
        return selected_move, alpha
    def alpha_beta_min(self, board, alpha, beta, depth_left):
        if (depth_left == 0): return None, -self.calculate_score(board)
        selected_move = None
        for move in board.legal_moves:
            board.push_uci(move.uci())
            _, score = self.alpha_beta_max(board, alpha, beta, depth_left-1)
            board.pop()
            if score <= alpha:
                return move.uci(), alpha
            if score < beta:
                beta = score
                selected_move = move.uci()
        return selected_move, beta
    def next_move(self, board: chess.Board, last_move: str, next_legal_moves: chess.LegalMoveGenerator):
        depth = 3
        #graph = self.construct_graph(board, depth)
        #move, score = self.minimax(graph)
        move, score = self.alpha_beta_max(board, float('-inf'), float('+inf'), depth)
        return move
    def reset(self, self_color):
        pass
    def terminate(self):
        pass
