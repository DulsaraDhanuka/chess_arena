from agents.random.agent import RandomAgent
#from agents.transformer.agent import TransformerAgent
from agents.minimax.agent import MinimaxAgent
import chess

agent1 = RandomAgent("random") 
agent2 = MinimaxAgent("Minimax")

board = chess.Board()
board.push_uci(agent2.next_move(board, None, board.legal_moves))

