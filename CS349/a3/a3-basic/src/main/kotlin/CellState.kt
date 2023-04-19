/**
 * CellState enumerates all possible states of the (public) board.
 */
enum class CellState {
    /**
     * This cell has not been attacked by the opposing player yet.
     */
    Ocean,

    /**
     * This cell has been attacked by the opposing player, and there was no ship.
     */
    Attacked,

    /**
     * This cell has been attacked by the opposing player, there was a ship, and the ship remains afloat.
     */
    ShipHit,

    /**
     * This cell has been attacked by the opposing player, there was a ship, and the ship has sunk.
     */
    ShipSunk;
}