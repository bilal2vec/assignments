import kotlin.random.Random

/**
 * A simple AI that plays Battleship.
 */
class AI(var game : Game) {

    init {
        game.gameStateProperty.addListener { _, _, newGameState ->
            when (newGameState) {
                GameState.AiSetup -> placeShips()
                GameState.AiAttack -> attackCell()
                else -> {}
            }
        }
    }

    private fun placeShips() {
        // for each ship that has to be placed
        game.getShipsToPlace().forEach {
            // try placing it at a random location with a random orientation
            while (game.placeShip(
                    it,
                    Orientation.values()[Random.nextInt(0, Orientation.values().size)],
                    Random.nextInt(0, game.dimension),
                    Random.nextInt(0, game.dimension)) == Cell.NoShip) ;
        }
        // signal that the AI has placed all its ships and is ready to start the game
        game.startGame()
    }

    private fun attackCell() {
        // create a list of cells that have not been attacked so far
        val attackedCell = game.getBoard(Player.Human)
            .flatten()
            .mapIndexed { idx, cellState -> idx to cellState }
            .filter { it.second == CellState.Ocean }
            .random()
        // attack one of the cells randomly
        game.attackCell(attackedCell.first % game.dimension, attackedCell.first / game.dimension)
    }
}