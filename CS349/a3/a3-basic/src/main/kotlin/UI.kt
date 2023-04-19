import javafx.animation.RotateTransition
import javafx.event.EventHandler
import javafx.geometry.Pos
import javafx.scene.control.*
import javafx.scene.input.MouseButton
import javafx.scene.input.MouseEvent
import javafx.scene.layout.*
import javafx.scene.transform.Rotate
import javafx.util.Duration
import jdk.jfr.Event
import kotlin.system.exitProcess


data class DragInfo(var target : Ship? = null,
    var anchorX: Double = 0.0,
    var anchorY: Double = 0.0,
    var offsetX: Double = 0.0,
    var offsetY: Double = 0.0,
    var initialX: Double = 0.0,
    var initialY: Double = 0.0)

class UI(private val game: Game) : HBox() {
    var my_formation = MyFormation(game)
    var my_fleet = MyFleet(game, my_formation)
    var start_game = Button("Start Game")
    var end_game = Button("End Game")
    var opponents_waters = OpponentsWaters(game)

    val computer = AI(game)

    fun add_event_handlers() {
        addEventFilter(MouseEvent.MOUSE_PRESSED) {
            if (game.gameState.value == GameState.HumanSetup) {
                if (game.current_ship != null && it.button == MouseButton.PRIMARY) {
                    val grid_bounds = my_formation.my_formation_grid.localToScreen(my_formation.my_formation_grid.boundsInLocal)
                    val grid_min_x = grid_bounds.minX + 30.0
                    val grid_min_y = grid_bounds.minY + 25.0

                    val ship_bounds = game.current_ship!!.localToScreen(game.current_ship!!.boundsInLocal)
                    val ship_min_x = ship_bounds.minX
                    val ship_min_y = ship_bounds.minY

                    var ship_height = 0.0
                    var ship_width = 0.0

                    if (game.current_ship!!.orientation == Orientation.Vertical) {
                        ship_height = game.current_ship!!.height
                        ship_width = game.current_ship!!.width
                    } else {
                        ship_height = game.current_ship!!.width
                        ship_width = game.current_ship!!.height
                    }

                    if (ship_min_x >= grid_min_x && (ship_min_x + ship_width) < (grid_min_x + 300.0) && ship_min_y >= grid_min_y && (ship_min_y + ship_height) < (grid_min_y + 300.0)) {

                        val relative_x = ship_min_x - grid_min_x
                        val relative_y = ship_min_y - grid_min_y

                        val x_idx = relative_x / 30.0
                        val y_idx = relative_y / 30.0

                        if (x_idx >= 0.0 && x_idx < 10.0 && y_idx >= 0.0 && y_idx < 10.0) {

                            val status = game.placeShip(game.current_ship!!.ship_type, game.current_ship!!.orientation, x_idx.toInt(), y_idx.toInt())

                            if (status != Cell.NoShip) {
                                game.current_ship!!.ship_id = status

                                game.current_ship!!.ship_placed = true
                                game.current_ship = null

                                my_formation.draw()

                                if (game.getShipsPlacedCount(Player.Human) == ShipType.values().count()) {
                                    start_game.isDisable = false
                                }
                            }
                        }
                    }
                }
            }
        }

        addEventFilter(MouseEvent.MOUSE_MOVED) {
            if (game.gameState.value == GameState.HumanSetup) {
                if (game.current_ship != null) {
                    game.current_ship!!.translateX = it.x - game.offsetX
                    game.current_ship!!.translateY = it.y - game.offsetY
                }
            }
        }
    }

    fun handle_start_game() {
        start_game.isDisable = true
        end_game.isDisable = false
        game.startGame()
    }

    fun handle_end_game() {
        start_game.isDisable = true
        end_game.isDisable = true
        exitProcess(0)
    }

    fun draw() {
        getChildren().removeAll()

        start_game.isDisable = true
        end_game.isDisable = false

        VBox.setVgrow(my_fleet, Priority.ALWAYS)

        var start_end_hbox = HBox(start_game, end_game)
        start_end_hbox.alignment = Pos.CENTER
        HBox.setHgrow(start_end_hbox, Priority.ALWAYS)

        var middle = VBox(my_fleet, start_end_hbox)

        var board = HBox(my_formation, middle, opponents_waters)
        HBox.setHgrow(my_formation, Priority.ALWAYS)
        HBox.setHgrow(middle, Priority.ALWAYS)
        HBox.setHgrow(opponents_waters, Priority.ALWAYS)

        start_game.onAction = EventHandler { handle_start_game() }
        end_game.onAction = EventHandler { handle_end_game() }

        add_event_handlers()

        children.add(board)
    }

    init {
        draw()
    }
}