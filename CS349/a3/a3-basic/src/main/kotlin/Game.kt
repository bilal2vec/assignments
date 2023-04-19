import javafx.beans.property.ReadOnlyObjectProperty
import javafx.beans.property.ReadOnlyObjectWrapper

/**
 * Game manages the rules of the game Battleship.
 * @param dimension number of rows and columns on the board
 * @param debug if true, debug messages are printed to the command line
 */
class Game (val dimension: Int, val debug: Boolean = false) {

    companion object {
        /**
         * A map between ship types and their length (in cells)
         */
        val shipLength = mapOf (
            ShipType.Battleship to 4,
            ShipType.Carrier to 5,
            ShipType.Cruiser to 3,
            ShipType.Destroyer to 2,
            ShipType.Submarine to 3,
        )
    }

    // a map with the two game boards
    val boards = mapOf(
        Player.Human to Board(dimension),
        Player.Ai to Board(dimension))

    // the currently active player, derived from [gameState]
    val activePlayer
        get() = when (gameState.value) {
            GameState.HumanSetup, GameState.HumanAttack -> Player.Human
            GameState.AiSetup, GameState.AiAttack -> Player.Ai
            else -> Player.None
        }

    // the current state of the game
    val gameState = ReadOnlyObjectWrapper(GameState.Init)

    /**
     * Indicates the current state of the game. Add listener to receive notifications about changes to the state.
     */
    val gameStateProperty: ReadOnlyObjectProperty<GameState> = gameState.readOnlyProperty

    var offsetX: Double = 0.0
    var offsetY: Double = 0.0

    var current_ship: Ship? = null

    /**
     * Attacks a cell on the opposing player's board.
     * @param x: the x-coordinate of the attacked cell
     * @param y: the y-coordinate of the attacked cell
     */
    fun attackCell(x: Int, y: Int)  {
        // attacks cell
        boards[activePlayer.other()]!!.attackCell(x, y)

        // check win-condition
        if (boards[activePlayer.other()]!!.hasShips().not()) {
            // if win, set game state accordingly
            gameState.value = when (activePlayer) {
                Player.Human -> GameState.HumanWon
                Player.Ai -> GameState.AiWon
                Player.None -> throw Exception("model.Game.attackCell..)")
            }
        } else {
            // if no win, set state to next player's attack
            gameState.value = gameState.value.next()
        }
    }

    /**
     * Places a ship for the currently active player.
     * @param shipType the type of the ship
     * @param orientation the orientation of the ship
     * @param bowX the x-coordinate of the cell in which the bow of the ship is located
     * @param bowY the y-coordinate of the cell in which the bow of the ship is located
     * @return the shipId of the newly placed ship, or [Cell.NoShip] if the ship was not placed
     */
    fun placeShip(shipType: ShipType, orientation: Orientation, bowX: Int, bowY: Int) : Int {
        if ((gameState.value == GameState.HumanSetup) or (gameState.value == GameState.AiSetup)) {
            return boards[activePlayer]!!.placeShip(shipType, orientation, bowX, bowY)
        }
        //if ((getShipsToPlace() - boards[player]!!.placedShips.map { ship -> ship.shipType}).contains(shipType).not()) {
        return Cell.NoShip
    }

    /**
     * Removes a ship from the board of the currently active player.
     * @param shipId the id of the ship to be removed
     */
    fun removeShip(shipId: Int) {
        if ((gameState.value == GameState.HumanSetup) or (gameState.value == GameState.AiSetup)) {
            boards[activePlayer]!!.removeShip(shipId)
        }
    }

    /**
     * Returns a list of all the ship that have to be placed.
     */
    fun getShipsToPlace() : List<ShipType> {
        // The returned list determines how many ships are in play. Traditionally, there is only one ship of each type.
        return listOf(
            ShipType.Destroyer,
            ShipType.Cruiser,
            ShipType.Submarine,
            ShipType.Battleship,
            ShipType.Carrier,
        )
    }

    /**
     * Returns the number of ships that player has currently placed.
     * @param player the player whose number of ships is queried
     * @return the number of ships for the player
     */
    fun getShipsPlacedCount(player: Player) : Int {
        return boards[player]!!.getPlacedShipCount()
    }

    /**
     * Returns if a particular ship for a player was sunk.
     * @param player the player whose ship is queried
     * @param shipId the id of the ship queried
     * @return true if the ship was sunk, and false otherwise
     */
    fun isSunk(player: Player, shipId: Int) : Boolean {
        return boards[player]!!.isSunk(shipId)
    }

    /**
     * Allows the current player to signal that they are ready to start the game, i.e., have completed their ship placement.
     */
    fun startGame() {
        when (gameState.value) {
            GameState.Init -> { gameState.value = GameState.HumanSetup }
            GameState.HumanSetup -> { gameState.value = GameState.AiSetup }
            GameState.AiSetup -> { gameState.value = GameState.HumanAttack }
            else -> {}
        }
    }

    /**
     * Returns the current state of the board for a player.
     * @param player the player whose board is queried
     * @return a 2d-list of all cell states; first (outer) dimension represents the y-coordinate, second (inner) one the x-coordinate
     */
    fun getBoard(player: Player) : List<List<CellState>> {
        if (debug) {
            println("$player's public board:\n${boards[player]!!.getBoardStates().fold("") { acc, cur -> "$acc${cur.fold("") { acc2, cur2 -> "$acc2\t$cur2" } }\n" }}\n$player's ")
        }

        return boards[player]!!.getBoardStates(debug)
    }
}
