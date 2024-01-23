import chess
import torch
from agents.transformer.model import Transformer
from agents.transformer.data_utils import Encoding

class TransformerAgent():
    def __init__(self, name):
        self.name = name
        self.enc = Encoding()
        self.context = [self.enc.encode('<s>')]

        n_vocab = self.enc.n_vocab
        block_size = 256
        n_embd = 384
        n_heads = 6
        n_blocks = 6
        dropout = 0.2
        self.device = 'cuda' if torch.cuda.is_available() else 'cpu'

        self.model = Transformer(block_size, n_vocab, n_embd, n_heads, n_blocks, dropout, self.device)
        self.model.to(self.device)
        self.model.load_state_dict(torch.load("agents/transformer/models/2/model-1705993302.7913043-11700.pth", map_location=torch.device('cpu')))
        self.model.eval()

    def next_move(self, board: chess.Board, last_move: str, next_legal_moves: chess.LegalMoveGenerator):
        if last_move is not None: self.context.append(self.enc.encode(last_move))
        while True:
            move_idx = self.model.generate(torch.tensor([self.context], dtype=torch.long, device=self.device), max_new_tokens=1)[0].tolist()[0]
            move = self.enc.decode(move_idx)
            try:
                if chess.Move.from_uci(move) in board.legal_moves:
                    self.context.append(move_idx)
                    return move
            except Exception as e:
                print(f"Context: {self.context}")
                print(f"Invalid move: {move}")
        return None

    def reset(self):
        pass

    def terminate(self):
        pass
