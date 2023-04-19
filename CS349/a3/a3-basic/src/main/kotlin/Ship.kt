import javafx.animation.Interpolator
import javafx.animation.RotateTransition
import javafx.animation.Transition
import javafx.animation.TranslateTransition
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
import kotlin.random.Random

class Ship(private val game: Game, private val ship: ShipType) : Rectangle() {
    var orientation = Orientation.Vertical
    var ship_type = ship
    var initial_x: Double = 0.0
    var initial_y: Double = 0.0
    var ship_placed = false
    var ship_id = -1

    init {
        width = 15.0
        height = 25.0 * Game.shipLength[ship]!!

        initial_x = x
        initial_y = y
    }
}