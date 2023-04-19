/**
 * Player enumerates all possibly active players.
 */
enum class Player {
    None { override fun other(): Player { return None } },
    Human { override fun other(): Player { return Ai } },
    Ai { override fun other(): Player { return Human } };

    abstract fun other() : Player
}