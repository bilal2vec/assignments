import javafx.animation.RotateTransition
import javafx.event.EventHandler
import javafx.geometry.Insets
import javafx.geometry.Pos
import javafx.scene.canvas.Canvas
import javafx.scene.control.*
import javafx.scene.input.MouseButton
import javafx.scene.input.MouseEvent
import javafx.scene.layout.*
import javafx.scene.paint.Color
import javafx.scene.shape.Rectangle
import javafx.scene.text.Font
import javafx.util.Duration
import java.security.KeyStore.TrustedCertificateEntry
import kotlin.random.Random


class MyFormation(private val game: Game) : VBox() {
    var my_formation_label = HBox(Label("My Formation"))
    var my_formation_grid = VBox()

    fun get_grid(): VBox {
        var my_board = game.boards[Player.Human]!!

        var board = VBox()

        val col_labels = arrayOf(" ", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J")
        var col_label_row = HBox()
        for (x in 0 until my_board.dimension + 1) {
            var col_label = Label(col_labels[x])
            col_label.minHeight = 25.0
            col_label.minWidth = 30.0
            col_label_row.children.add(col_label)
        }
        board.children.add(col_label_row)

        for (y in 0 until my_board.dimension) {
            var row = HBox()
            var row_label = Label((y+1).toString())
            row_label.minHeight = 30.0
            row_label.minWidth = 30.0
            row.children.add(row_label)
            for (x in 0 until my_board.dimension) {
                val cell = my_board.cells.find { (it.x == x) and (it.y == y) }!!
                val cellView = Rectangle(29.0, 29.0)

                if (game.gameState.value == GameState.AiWon || game.gameState.value == GameState.HumanWon) {
                    cellView.fill = when {
                        my_board.isSunk(cell.shipId) -> Color.PURPLE
                        else -> Color.LIGHTBLUE
                    }
                } else {
                    cellView.fill = when {
                        my_board.isSunk(cell.shipId) -> Color.PURPLE
                        cell.wasAttacked and (cell.shipId != Cell.NoShip) -> Color.RED
                        cell.wasAttacked -> Color.GRAY
                        cell.shipId != Cell.NoShip -> Color.BLUE
                        else -> Color.LIGHTBLUE
                    }
                }

                cellView.stroke = Color.BLACK
                cellView.strokeWidth = 1.0
                row.children.add(cellView)
            }
            board.children.add(row)
        }

        return board
    }

    fun draw() {
        getChildren().clear()

        HBox.setHgrow(my_formation_label, Priority.ALWAYS)
        my_formation_label.alignment = Pos.CENTER

        my_formation_grid = get_grid()

        children.add(my_formation_label)
        children.add(my_formation_grid)
    }

    init {
        game.gameStateProperty.addListener { _, _, newGameState ->
            if (newGameState == GameState.HumanWon) {
                my_formation_label = HBox(Label("You Won!"))
            } else if (newGameState == GameState.AiWon) {
                my_formation_label = HBox(Label("You Were Defeated!"))
            }
            draw()
        }

        draw()
    }
}