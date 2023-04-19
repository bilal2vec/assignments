import javafx.application.Application
import javafx.scene.Scene
import javafx.scene.layout.Pane
import javafx.stage.Stage

class Battleship : Application() {
    override fun start(stage: Stage) {
        stage.title = "CS349 - A3 Battleship - b54khan"

        stage.width = 875.0
        stage.height = 375.0
        stage.minWidth = 875.0
        stage.minHeight = 375.0

        val game = Game(10)
        game.startGame()

        val ui = UI(game)

        stage.scene = Scene(ui)
        stage.show()
    }
}