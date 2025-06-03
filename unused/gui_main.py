import sys
import os
from PyQt5.QtWidgets import QApplication, QLabel, QWidget, QGridLayout, QMainWindow, QMessageBox
from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import Qt
import subprocess

def square_to_index(square):
    row, col = square_to_coords(square)
    return row * 8 + col

def square_to_bitboard(square):
    row, col = square_to_coords(square)
    index = row * 8 + col
    return 1 << index

def coords_to_square(row, col):
    return chr(ord('a') + col) + str(8 - row)

def square_to_coords(square):
    return 8 - int(square[1]), ord(square[0]) - ord('a')

class ChessPiece(QLabel):
    def __init__(self, image_path, color, position, parent_square, game_ref):
        super().__init__(parent_square)
        self.image_path = image_path
        self.color = color
        self.position = position
        self.game = game_ref

        pixmap = QPixmap(image_path)
        if pixmap.isNull():
            print(f"Error: could not load image {image_path}")
        else:
            self.setPixmap(pixmap.scaled(75, 75, Qt.KeepAspectRatio, Qt.SmoothTransformation))

        self.setFixedSize(80, 80)
        self.setAttribute(Qt.WA_DeleteOnClose)

    def mousePressEvent(self, event):
        if self.color != self.game.player_color:
            return

        if self.position:
            square_idx = square_to_index(self.position)
            moves = self.game.get_valid_moves(square_idx)
            self.game.highlight_moves(moves, self)
            event.accept()


class ChessSquare(QLabel):
    def __init__(self, row, col, board_ref):
        super().__init__()
        self.row = row
        self.col = col
        self.board = board_ref
        self.piece = None
        self.setFixedSize(80, 80)
        self.setAlignment(Qt.AlignCenter)
        self.setAcceptDrops(True)
        self.reset_style()

    def reset_style(self):
        light = (self.row + self.col) % 2 == 0
        color = '#f0d9b5' if light else '#b58863'
        self.setStyleSheet(f"background-color: {color}")

    def highlight(self):
        self.setStyleSheet("background-color: #88ff88;")

    def mousePressEvent(self, event):
        if self in self.board.highlighted:
            self.board.make_move(self)


class ChessBoard(QWidget):   
    def __init__(self, player_color):
        super().__init__()
        self.engine = subprocess.Popen(
        ["engine/b"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
        )

        self.grid = QGridLayout(self)
        self.grid.setSpacing(0)
        self.setFixedSize(640, 640)
        self.player_color = player_color
        self.squares = [[None for _ in range(8)] for _ in range(8)]
        self.highlighted = []
        self.selected_piece = None
        self.init_board(player_color)

    def init_board(self, player_color):
        for row in range(8):
            for col in range(8):
                square = ChessSquare(row, col, self)
                self.grid.addWidget(square, row, col)
                self.squares[row][col] = square
        self.setup_pieces(player_color)

    def setup_pieces(self, player_color):
        enemy_color = "black" if player_color == "white" else "white"
        # Pawns
        for col in range(8):
            self.place_piece(1, col, f"assets/{enemy_color[0]}pawn.png", enemy_color)
            self.place_piece(6, col, f"assets/{player_color[0]}pawn.png", player_color)
        # Major pieces
        if player_color == "white":
            for col, piece in enumerate(["rook", "knight", "bishop", "queen", "king", "bishop", "knight", "rook"]):
                self.place_piece(0, col, f"assets/{enemy_color[0]}{piece}.png", enemy_color)
                self.place_piece(7, col, f"assets/{player_color[0]}{piece}.png", player_color)
        else:
            for col, piece in enumerate(["rook", "knight", "bishop", "king", "queen", "bishop", "knight", "rook"]):
                self.place_piece(0, col, f"assets/{enemy_color[0]}{piece}.png", enemy_color)
                self.place_piece(7, col, f"assets/{player_color[0]}{piece}.png", player_color)

    def place_piece(self, row, col, image_path, color):
        square = self.squares[row][col]
        if square.piece:
            square.piece.close()
            square.piece = None

        position = coords_to_square(row, col)
        piece = ChessPiece(image_path, color, position, square, self)
        piece.move(0, 0)
        piece.show()
        square.piece = piece

    def highlight_moves(self, squares, piece):
        self.clear_highlights()
        self.selected_piece = piece
        for sq in squares:
            row, col = square_to_coords(sq)
            square = self.squares[row][col]
            square.highlight()
            self.highlighted.append(square)

    def clear_highlights(self):
        for square in self.highlighted:
            square.reset_style()
        self.highlighted = []

    def make_move(self, target_square):
        from_square = self.selected_piece.position
        to_square = coords_to_square(target_square.row, target_square.col)

        filename = os.path.basename(self.selected_piece.image_path)
        piece_type = os.path.splitext(filename)[0][1:] 

        from_idx = square_to_index(from_square)
        to_idx = square_to_index(to_square)

        msg = f"{piece_type} {from_idx} {to_idx}\n"
        print(f"Sending: {msg.strip()}")
        self.engine.stdin.write(msg)
        self.engine.stdin.flush()

        if target_square.piece:
            target_square.piece.close()
            target_square.piece = None

        source_square = self.selected_piece.parent()
        source_square.piece = None

        target_square.piece = self.selected_piece
        self.selected_piece.setParent(target_square)
        self.selected_piece.move(0, 0)
        self.selected_piece.position = to_square
        self.selected_piece.show()

        self.selected_piece = None
        self.clear_highlights()

        response = self.engine.stdout.readline().strip()
        print(f"Engine move: {response}")
        from_idx, to_idx = map(int, response.split())
        from_row, from_col = divmod(from_idx, 8)
        to_row, to_col = divmod(to_idx, 8)
        self.move_piece_gui(from_row, from_col, to_row, to_col)

    def move_piece_gui(self, from_row, from_col, to_row, to_col):
        from_square = self.squares[from_row][from_col]
        to_square = self.squares[to_row][to_col]
        if from_square.piece:
            piece = from_square.piece
            from_square.piece = None
            if to_square.piece:
                to_square.piece.close()
            to_square.piece = piece
            piece.setParent(to_square)
            piece.move(0, 0)
            piece.position = coords_to_square(to_row, to_col)
            piece.show()
        # Optional: read response
        # response = self.engine.stdout.readline().strip()
        # print(f"Engine says: {response}")

    def get_valid_moves(self, square_index: int) -> list[str]:
        self.engine.stdin.write(f"{square_index}\n")
        self.engine.stdin.flush()

        moves = []
        while True:
            line = self.engine.stdout.readline()
            if not line:
                print("Engine output EOF or closed pipe")
                break
            line = line.strip()
            print(f"Received line from engine: '{line}'")  # Debug print
            if line == "":
                break
            try:
                idx = int(line)
                moves.append(coords_to_square(idx // 8, idx % 8))
            except ValueError:
                print(f"Unexpected engine output: {line}")
                continue
        return moves


class MainWindow(QMainWindow):
    def ask_player_color(self):
        msg = QMessageBox(self)
        msg.setWindowTitle("Choose Color")
        msg.setText("Select your color:")
        white_button = msg.addButton("White", QMessageBox.AcceptRole)
        black_button = msg.addButton("Black", QMessageBox.RejectRole)
        msg.exec_()
        return "white" if msg.clickedButton() == white_button else "black"

    def __init__(self):
        super().__init__()
        self.setWindowTitle("Chess")
        self.setFixedSize(640, 640)
        self.player_color = self.ask_player_color()
        self.chessboard = ChessBoard(self.player_color)
        self.setCentralWidget(self.chessboard)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())


