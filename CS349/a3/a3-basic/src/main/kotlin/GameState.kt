/**
 * GameState enumerates all possible states of the game.
 */
enum class GameState {
    /**
     * Init is the game state at the beginning of the game. At this point, there is no active player.
     */
    Init { override fun next(): GameState { return HumanSetup } },

    /**
     * HumanSetup indicates that it is the human player's turn to set up the board, i.e., placing their ships.
     */
    HumanSetup { override fun next(): GameState { return AiSetup } },

    /**
     * AiSetup indicates that it is the computer's turn to set up the board, i.e., placing its ships.
     */
    AiSetup { override fun next(): GameState { return HumanAttack } },

    /**
     * HumanAttack indicates that it is the human player's turn to attack a cell on the computer's board.
     */
    HumanAttack { override fun next(): GameState { return AiAttack } },

    /**
     * AiAttack indicates that it is the computer's turn to attack a cell on the human player's board.
     */
    AiAttack { override fun next(): GameState { return HumanAttack } },

    /**
     * HumanWin indicates that the human player has won the game. The game is over, there is no active player.
     */
    HumanWon  { override fun next(): GameState { return HumanWon } },

    /**
     * AiWon indicates that the computer has won the game. The game is over, there is no active player.
     */
    AiWon  { override fun next(): GameState { return AiWon } };

    /**
     * The function progresses to the next game state.
     */
    abstract fun next() : GameState
}