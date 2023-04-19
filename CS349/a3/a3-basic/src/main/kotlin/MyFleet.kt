import javafx.animation.RotateTransition
import javafx.geometry.Insets
import javafx.geometry.Pos
import javafx.scene.Node
import javafx.scene.control.*
import javafx.scene.input.MouseButton
import javafx.scene.input.MouseEvent
import javafx.scene.layout.*
import javafx.scene.paint.Color
import javafx.scene.transform.Rotate
import javafx.util.Duration


class MyFleet(private val game: Game, var myFormation: MyFormation) : VBox() {
    var my_fleet_label = Label("My Fleet")

    fun add_event_handlers(fleet_list: HBox) {
        for (node in fleet_list.children) {
            if (node is Ship) {
                val ship = (node as Ship)

                ship.addEventFilter(MouseEvent.MOUSE_PRESSED) {
                    if (game.gameState.value == GameState.HumanSetup) {
                        if (game.current_ship == null && ship.ship_placed == false) {
                            game.current_ship = ship
                            game.offsetX = it.sceneX
                            game.offsetY = it.sceneY

                        } else if (game.current_ship == ship) {
                            if (it.button == MouseButton.SECONDARY) {
                                if (ship.orientation == Orientation.Vertical) {
                                    ship.transforms.add(Rotate(90.0, it.x, it.y))
                                    ship.orientation = Orientation.Horizontal
                                } else if (ship.orientation == Orientation.Horizontal) {
                                    ship.transforms.add(Rotate(-90.0, it.x, it.y))
                                    ship.orientation = Orientation.Vertical
                                }
                            } else if (it.button == MouseButton.PRIMARY) {
                                if (ship.orientation == Orientation.Horizontal) {
                                    ship.transforms.add(Rotate(-90.0, it.x, it.y))
                                    ship.orientation = Orientation.Vertical
                                }

                                ship.translateX = ship.initial_x
                                ship.translateY = ship.initial_y
                                game.current_ship = null
                            }
                        }
                    }
                }
            }
        }
    }

    init {
        var my_fleet_hbox = HBox(my_fleet_label)

        my_fleet_hbox.alignment = Pos.CENTER
        HBox.setHgrow(my_fleet_hbox, Priority.ALWAYS)

        children.add(my_fleet_hbox)
        var fleet_list = HBox()

        val spacer = HBox()
        HBox.setHgrow(spacer, Priority.ALWAYS)
        fleet_list.children.add(spacer)

        HBox.setHgrow(fleet_list, Priority.ALWAYS)
        fleet_list.alignment = Pos.CENTER

        fleet_list.minWidth = 150.0

        var i = 0
        for (s in game.getShipsToPlace()) {
            val ship = Ship(game, s)
            ship.ship_type = s

            fleet_list.children.add(ship)

            val spacer = HBox()
            HBox.setHgrow(spacer, Priority.ALWAYS)
            fleet_list.children.add(spacer)
        }

        add_event_handlers(fleet_list)

        game.gameStateProperty.addListener { _, _, newGameState ->
            if (game.gameState.value == GameState.AiWon || game.gameState.value == GameState.HumanWon) {
                for (node in fleet_list.children) {
                    if (node is Ship) {
                        val ship = (node as Ship)

                        if (!game.isSunk(Player.Human, ship.ship_id)) {
                            game.boards!![Player.Ai]!!.removeShip(i)
                            ship.translateX = ship.initial_x
                            ship.translateY = ship.initial_y
                        }
                    }
                }
            }
        }


        children.add(fleet_list)

    }
}