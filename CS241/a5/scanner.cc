#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <utility>
#include <set>
#include <array>
#include "scanner.h"

Token::Token(Token::Kind kind, std::string lexeme) : kind(kind), lexeme(std::move(lexeme)) {}

Token::Kind Token::getKind() const { return kind; }
const std::string &Token::getLexeme() const { return lexeme; }

std::ostream &operator<<(std::ostream &out, const Token &tok)
{
    switch (tok.getKind())
    {
    case Token::ID:
        out << "ID";
        break;
    case Token::NUM:
        out << "NUM";
        break;
    case Token::LPAREN:
        out << "LPAREN";
        break;
    case Token::RPAREN:
        out << "RPAREN";
        break;
    case Token::LBRACE:
        out << "LBRACE";
        break;
    case Token::RBRACE:
        out << "RBRACE";
        break;
    case Token::RETURN:
        out << "RETURN";
        break;
    case Token::IF:
        out << "IF";
        break;
    case Token::ELSE:
        out << "ELSE";
        break;
    case Token::WHILE:
        out << "WHILE";
        break;
    case Token::PRINTLN:
        out << "PRINTLN";
        break;
    case Token::WAIN:
        out << "WAIN";
        break;
    case Token::BECOMES:
        out << "BECOMES";
        break;
    case Token::INT:
        out << "INT";
        break;
    case Token::EQ:
        out << "EQ";
        break;
    case Token::NE:
        out << "NE";
        break;
    case Token::LT:
        out << "LT";
        break;
    case Token::GT:
        out << "GT";
        break;
    case Token::LE:
        out << "LE";
        break;
    case Token::GE:
        out << "GE";
        break;
    case Token::PLUS:
        out << "PLUS";
        break;
    case Token::MINUS:
        out << "MINUS";
        break;
    case Token::STAR:
        out << "STAR";
        break;
    case Token::SLASH:
        out << "SLASH";
        break;
    case Token::PCT:
        out << "PCT";
        break;
    case Token::COMMA:
        out << "COMMA";
        break;
    case Token::SEMI:
        out << "SEMI";
        break;
    case Token::NEW:
        out << "NEW";
        break;
    case Token::DELETE:
        out << "DELETE";
        break;
    case Token::LBRACK:
        out << "LBRACK";
        break;
    case Token::RBRACK:
        out << "RBRACK";
        break;
    case Token::AMP:
        out << "AMP";
        break;
    case Token::NULLTOKEN:
        out << "NULL";
        break;
    case Token::SPACE:
        out << "SPACE";
        break;
    case Token::TAB:
        out << "TAB";
        break;
    case Token::COMMENT:
        out << "COMMENT";
        break;
    }
    out << " " << tok.getLexeme();

    return out;
}

int64_t Token::toNumber() const
{
    std::istringstream iss;
    int64_t result;

    if (kind == NUM)
    {
        iss.str(lexeme);
    }
    else
    {
        // This should never happen if the user calls this function correctly
        return 0;
    }

    iss >> result;
    return result;
}

ScanningFailure::ScanningFailure(std::string message) : message(std::move(message)) {}

const std::string &ScanningFailure::what() const { return message; }

class WLP4DFA
{
public:
    enum State
    {
        ID = 0,
        NUM,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        RETURN,
        IF,
        ELSE,
        WHILE,
        PRINTLN,
        WAIN,
        BECOMES,
        INT,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE,
        PLUS,
        MINUS,
        STAR,
        SLASH,
        PCT,
        COMMA,
        SEMI,
        NEW,
        DELETE,
        LBRACK,
        RBRACK,
        AMP,
        NULLTOKEN,
        SPACE,
        TAB,
        COMMENT,

        NE_PART_1,
        ZERO,

        RETURN_R,
        RETURN_RE,
        RETURN_RET,
        RETURN_RETU,
        RETURN_RETUR,

        IF_INT_I,
        INT_IN,

        ELSE_E,
        ELSE_EL,
        ELSE_ELS,

        WAIN_WHILE_W,
        WAIN_WA,
        WAIN_WAI,
        WHILE_WH,
        WHILE_WHI,
        WHILE_WHIL,

        PRINTLN_P,
        PRINTLN_PR,
        PRINTLN_PRI,
        PRINTLN_PRIN,
        PRINTLN_PRINT,
        PRINTLN_PRINTL,

        DELETE_D,
        DELETE_DE,
        DELETE_DEL,
        DELETE_DELE,
        DELETE_DELET,

        NULL_N,
        NULL_NU,
        NULL_NUL,

        NEW_N,
        NEW_NE,

        START,
        FAIL,

        LARGEST_STATE = FAIL
    };

private:
    std::set<State> acceptingStates;
    std::array<std::array<State, 128>, LARGEST_STATE + 1> transitionFunction;

    Token::Kind stateToKind(State s) const
    {
        switch (s)
        {
        case ID:
            return Token::ID;
        case NUM:
            return Token::NUM;
        case LPAREN:
            return Token::LPAREN;
        case RPAREN:
            return Token::RPAREN;
        case LBRACE:
            return Token::LBRACE;
        case RBRACE:
            return Token::RBRACE;
        case RETURN:
            return Token::RETURN;
        case IF:
            return Token::IF;
        case ELSE:
            return Token::ELSE;
        case WHILE:
            return Token::WHILE;
        case PRINTLN:
            return Token::PRINTLN;
        case WAIN:
            return Token::WAIN;
        case BECOMES:
            return Token::BECOMES;
        case INT:
            return Token::INT;
        case EQ:
            return Token::EQ;
        case NE:
            return Token::NE;
        case LT:
            return Token::LT;
        case GT:
            return Token::GT;
        case LE:
            return Token::LE;
        case GE:
            return Token::GE;
        case PLUS:
            return Token::PLUS;
        case MINUS:
            return Token::MINUS;
        case STAR:
            return Token::STAR;
        case SLASH:
            return Token::SLASH;
        case PCT:
            return Token::PCT;
        case COMMA:
            return Token::COMMA;
        case SEMI:
            return Token::SEMI;
        case NEW:
            return Token::NEW;
        case DELETE:
            return Token::DELETE;
        case LBRACK:
            return Token::LBRACK;
        case RBRACK:
            return Token::RBRACK;
        case AMP:
            return Token::AMP;
        case NULLTOKEN:
            return Token::NULLTOKEN;
        case SPACE:
            return Token::SPACE;
        case TAB:
            return Token::TAB;
        case COMMENT:
            return Token::COMMENT;
        case ZERO:
            return Token::NUM;
        case RETURN_R:
            return Token::ID;
        case RETURN_RE:
            return Token::ID;
        case RETURN_RET:
            return Token::ID;
        case RETURN_RETU:
            return Token::ID;
        case RETURN_RETUR:
            return Token::ID;
        case IF_INT_I:
            return Token::ID;
        case INT_IN:
            return Token::ID;
        case ELSE_E:
            return Token::ID;
        case ELSE_EL:
            return Token::ID;
        case ELSE_ELS:
            return Token::ID;
        case WAIN_WHILE_W:
            return Token::ID;
        case WAIN_WA:
            return Token::ID;
        case WAIN_WAI:
            return Token::ID;
        case WHILE_WH:
            return Token::ID;
        case WHILE_WHI:
            return Token::ID;
        case WHILE_WHIL:
            return Token::ID;
        case PRINTLN_P:
            return Token::ID;
        case PRINTLN_PR:
            return Token::ID;
        case PRINTLN_PRI:
            return Token::ID;
        case PRINTLN_PRIN:
            return Token::ID;
        case PRINTLN_PRINT:
            return Token::ID;
        case PRINTLN_PRINTL:
            return Token::ID;
        case DELETE_D:
            return Token::ID;
        case DELETE_DE:
            return Token::ID;
        case DELETE_DEL:
            return Token::ID;
        case DELETE_DELE:
            return Token::ID;
        case DELETE_DELET:
            return Token::ID;
        case NULL_N:
            return Token::ID;
        case NULL_NU:
            return Token::ID;
        case NULL_NUL:
            return Token::ID;
        case NEW_N:
            return Token::ID;
        case NEW_NE:
            return Token::ID;
        default:
            throw ScanningFailure("ERROR: Cannot convert state to kind.");
        }
    }

public:
    std::vector<Token> simplifiedMaximalMunch(const std::string &input) const
    {
        std::vector<Token> result;

        State state = start();
        std::string munchedInput;

        for (std::string::const_iterator inputPosn = input.begin();
             inputPosn != input.end();)
        {

            State oldState = state;
            state = transition(state, *inputPosn);

            if (!failed(state))
            {
                munchedInput += *inputPosn;
                oldState = state;

                ++inputPosn;
            }

            if (inputPosn == input.end() || failed(state))
            {
                if (accept(oldState))
                {
                    result.push_back(Token(stateToKind(oldState), munchedInput));

                    munchedInput = "";
                    state = start();
                }
                else
                {
                    if (failed(state))
                    {
                        munchedInput += *inputPosn;
                    }
                    throw ScanningFailure("ERROR: Simplified maximal munch failed on input: " + munchedInput);
                }
            }
        }

        return result;
    }

    /* Initializes the accepting states for the DFA.
     */
    WLP4DFA()
    {
        acceptingStates = {
            ID,
            NUM,
            LPAREN,
            RPAREN,
            LBRACE,
            RBRACE,
            RETURN,
            IF,
            ELSE,
            WHILE,
            PRINTLN,
            WAIN,
            BECOMES,
            INT,
            EQ,
            NE,
            LT,
            GT,
            LE,
            GE,
            PLUS,
            MINUS,
            STAR,
            SLASH,
            PCT,
            COMMA,
            SEMI,
            NEW,
            DELETE,
            LBRACK,
            RBRACK,
            AMP,
            NULLTOKEN,
            SPACE,
            TAB,
            COMMENT,
            ZERO,
            RETURN_R,
            RETURN_RE,
            RETURN_RET,
            RETURN_RETU,
            RETURN_RETUR,

            IF_INT_I,
            INT_IN,

            ELSE_E,
            ELSE_EL,
            ELSE_ELS,

            WAIN_WHILE_W,
            WAIN_WA,
            WAIN_WAI,
            WHILE_WH,
            WHILE_WHI,
            WHILE_WHIL,

            PRINTLN_P,
            PRINTLN_PR,
            PRINTLN_PRI,
            PRINTLN_PRIN,
            PRINTLN_PRINT,
            PRINTLN_PRINTL,

            DELETE_D,
            DELETE_DE,
            DELETE_DEL,
            DELETE_DELE,
            DELETE_DELET,

            NULL_N,
            NULL_NU,
            NULL_NUL,

            NEW_N,
            NEW_NE,
        };

        for (size_t i = 0; i < transitionFunction.size(); ++i)
        {
            for (size_t j = 0; j < transitionFunction[0].size(); ++j)
            {
                transitionFunction[i][j] = FAIL;
            }
        }

        registerTransition(START, "abcfghjklmoqstuvxyzABCDEFGHIJKLMOPQRSTUVWXYZ", ID);
        registerTransition(START, "d", DELETE_D);
        registerTransition(START, "e", ELSE_E);
        registerTransition(START, "i", IF_INT_I);
        registerTransition(START, "n", NEW_N);
        registerTransition(START, "p", PRINTLN_P);
        registerTransition(START, "r", RETURN_R);
        registerTransition(START, "w", WAIN_WHILE_W);
        registerTransition(START, "N", NULL_N);

        registerTransition(START, "w", WAIN_WHILE_W);
        registerTransition(START, "0", ZERO);
        registerTransition(START, "123456789", NUM);
        registerTransition(START, "(", LPAREN);
        registerTransition(START, ")", RPAREN);
        registerTransition(START, "{", LBRACE);
        registerTransition(START, "}", RBRACE);
        registerTransition(START, "=", BECOMES);
        registerTransition(BECOMES, "=", EQ);
        registerTransition(START, "!", NE_PART_1);
        registerTransition(NE_PART_1, "=", NE);
        registerTransition(START, "<", LT);
        registerTransition(START, ">", GT);
        registerTransition(LT, "=", LE);
        registerTransition(GT, "=", GE);
        registerTransition(START, "+", PLUS);
        registerTransition(START, "-", MINUS);
        registerTransition(START, "*", STAR);
        registerTransition(START, "/", SLASH);
        registerTransition(START, "%", PCT);
        registerTransition(START, ",", COMMA);
        registerTransition(START, ";", SEMI);
        registerTransition(START, "[", LBRACK);
        registerTransition(START, "]", RBRACK);
        registerTransition(START, "&", AMP);
        registerTransition(START, isspace, SPACE);
        registerTransition(START, "\t", TAB);
        registerTransition(SLASH, "/", COMMENT);
        registerTransition(
            COMMENT, [](int c) -> int
            { return c != '\n'; },
            COMMENT);

        registerTransition(NUM, isdigit, NUM);
        registerTransition(ID, isalnum, ID);

        registerTransition(DELETE_D, "abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", ID);
        registerTransition(DELETE_D, "e", DELETE_DE);
        registerTransition(DELETE_DE, "l", DELETE_DEL);
        registerTransition(DELETE_DEL, "e", DELETE_DELE);
        registerTransition(DELETE_DELE, "t", DELETE_DELET);
        registerTransition(DELETE_DELET, "e", DELETE);
        registerTransition(DELETE, isalnum, ID);

        registerTransition(ELSE_E, "abcdefghijkmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", ID);
        registerTransition(ELSE_E, "l", ELSE_EL);
        registerTransition(ELSE_EL, "s", ELSE_ELS);
        registerTransition(ELSE_ELS, "e", ELSE);
        registerTransition(ELSE, isalnum, ID);

        registerTransition(IF_INT_I, "abcdefghjklmopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", ID);
        registerTransition(IF_INT_I, "f", IF);
        registerTransition(IF, isalnum, ID);
        registerTransition(IF_INT_I, "n", INT_IN);
        registerTransition(INT_IN, "t", INT);
        registerTransition(INT, isalnum, ID);

        registerTransition(NEW_N, "abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", ID);
        registerTransition(NEW_N, "e", NEW_NE);
        registerTransition(NEW_NE, "w", NEW);
        registerTransition(NEW, isalnum, ID);

        registerTransition(PRINTLN, "abcdefghijklmnopqstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", ID);
        registerTransition(PRINTLN_P, "r", PRINTLN_PR);
        registerTransition(PRINTLN_PR, "i", PRINTLN_PRI);
        registerTransition(PRINTLN_PRI, "n", PRINTLN_PRIN);
        registerTransition(PRINTLN_PRIN, "t", PRINTLN_PRINT);
        registerTransition(PRINTLN_PRINT, "l", PRINTLN_PRINTL);
        registerTransition(PRINTLN_PRINTL, "n", PRINTLN);
        registerTransition(PRINTLN, isalnum, ID);

        registerTransition(RETURN_R, "abcdfghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", ID);
        registerTransition(RETURN_R, "e", RETURN_RE);
        registerTransition(RETURN_RE, "t", RETURN_RET);
        registerTransition(RETURN_RET, "u", RETURN_RETU);
        registerTransition(RETURN_RETU, "r", RETURN_RETUR);
        registerTransition(RETURN_RETUR, "n", RETURN);
        registerTransition(RETURN, isalnum, ID);

        registerTransition(WAIN_WHILE_W, "bcdefgijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", ID);
        registerTransition(WAIN_WHILE_W, "a", WAIN_WA);
        registerTransition(WAIN_WA, "i", WAIN_WAI);
        registerTransition(WAIN_WAI, "n", WAIN);
        registerTransition(WAIN, isalnum, ID);
        registerTransition(WAIN_WHILE_W, "h", WHILE_WH);
        registerTransition(WHILE_WH, "i", WHILE_WHI);
        registerTransition(WHILE_WHI, "l", WHILE_WHIL);
        registerTransition(WHILE_WHIL, "e", WHILE);
        registerTransition(WHILE, isalnum, ID);

        registerTransition(NULL_N, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTVWXYZ0123456789", ID);
        registerTransition(NULL_N, "U", NULL_NU);
        registerTransition(NULL_NU, "L", NULL_NUL);
        registerTransition(NULL_NUL, "L", NULLTOKEN);
        registerTransition(NULLTOKEN, isalnum, ID);
    }

    // Register a transition on all chars in chars
    void registerTransition(State oldState, const std::string &chars,
                            State newState)
    {
        for (char c : chars)
        {
            transitionFunction[oldState][c] = newState;
        }
    }

    // Register a transition on all chars matching test
    // For some reason the cctype functions all use ints, hence the function
    // argument type.
    void registerTransition(State oldState, int (*test)(int), State newState)
    {

        for (int c = 0; c < 128; ++c)
        {
            if (test(c))
            {
                transitionFunction[oldState][c] = newState;
            }
        }
    }

    /* Returns the state corresponding to following a transition
     * from the given starting state on the given character,
     * or a special fail state if the transition does not exist.
     */
    State transition(State state, char nextChar) const
    {
        return transitionFunction[state][nextChar];
    }

    /* Checks whether the state returned by transition
     * corresponds to failure to transition.
     */
    bool failed(State state) const { return state == FAIL; }

    /* Checks whether the state returned by transition
     * is an accepting state.
     */
    bool accept(State state) const
    {
        return acceptingStates.count(state) > 0;
    }

    /* Returns the starting state of the DFA
     */
    State start() const { return START; }
};

std::vector<Token> scan(const std::string &input)
{
    static WLP4DFA theDFA;

    std::vector<Token> tokens = theDFA.simplifiedMaximalMunch(input);

    std::vector<Token> newTokens;

    for (auto &token : tokens)
    {
        if ((token.getKind() != Token::SPACE) && (token.getKind() != Token::TAB) && (token.getKind() != Token::COMMENT))
        {

            if (token.getKind() == Token::NUM)
            {
                int64_t value = token.toNumber();
                if (value > 2147483647)
                {
                    throw ScanningFailure("ERROR: number larger than 2^31 - 1");
                }
                else
                {
                    newTokens.push_back(token);
                }
            }
            else
            {
                newTokens.push_back(token);
            }
        }
    }

    return newTokens;
}
