import pickle

vocab = None
with open('vocab.pickle', "rb") as f:
    vocab = pickle.load(f)
n_vocab = len(vocab.items()) // 2

class Encoding():
    def __init__(self) -> None:
        self.n_vocab = n_vocab
    def encode(self, move: str): return vocab[move]
    def decode(self, move: int): return vocab[move]
