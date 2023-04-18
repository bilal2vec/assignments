Object subclass: LambdaExpr [
    isVar [ ^false ]
    isAbs [ ^false ]
    isApp [ ^false ]

    ifVar: varBlock ifAbs: absBlock ifApp: appBlock [
        self isVar ifTrue: [
            ^varBlock value
        ].
        self isAbs ifTrue: [
            ^absBlock value
        ].
        self isApp ifTrue: [
            ^appBlock value
        ].
        self error: 'Invalid type'
    ]

    reduceWith: block steps: ssteps [
        | exp steps next |
        exp := self.
        steps := ssteps.
        [ steps > 0 ] whileTrue: [
            next := block value: exp.
            next isNil ifTrue: [ ^exp ].
            exp := next.
            steps := steps - 1.
        ].
        ^exp
    ]

    freeVars [
        ^self freeVars: (Dictionary new)
    ]

    printString [
        ^self displayString
    ]
]

LambdaExpr subclass: LambdaVar [
    | name |

    LambdaVar class >> withName: name [
        | r |
        r := super new.
        r init: name.
        ^r
    ]

    init: sname [
        name := sname.
    ]

    dup [
        ^LambdaVar withName: name
    ]

    isVar [ ^true ]

    name [
        ^name
    ]

    freeVars: map [
        map at: name put: self.
        ^map
    ]

    displayString [
        ^(name asString)
    ]
]

LambdaExpr subclass: LambdaAbs [
    | var body |

    LambdaAbs class >> withVar: var body: body [
        | r |
        r := super new.
        r init: var body: body.
        ^r
    ]

    init: svar body: sbody [
        var := svar.
        body := sbody.
    ]

    dup [
        ^LambdaAbs withVar: var body: body dup
    ]

    isAbs [ ^true ]

    var [
        ^var
    ]

    body [
        ^body
    ]

    freeVars: map [
        | hadKey |
        hadKey := map includesKey: var.
        body freeVars: map.
        hadKey ifFalse: [ map removeKey: var ifAbsent: [nil]. ].
        ^map
    ]

    displayString [
        ^'(^' , var , '.' , body displayString , ')'
    ]
]

LambdaExpr subclass: LambdaApp [
    | rator rand |

    LambdaApp class >> withRator: rator rand: rand [
        | r |
        r := super new.
        r init: rator rand: rand.
        ^r
    ]

    init: srator rand: srand [
        rator := srator.
        rand := srand.
    ]

    dup [
        ^LambdaApp withRator: rator dup rand: rand dup
    ]

    isApp [ ^true ]

    rator [
        ^rator
    ]

    rand [
        ^rand
    ]

    freeVars: map [
        rator freeVars: map.
        rand freeVars: map.
        ^map
    ]

    displayString [
        ^'(' , rator displayString , ' ' , rand displayString , ')'
    ]
]

Object subclass: LambdaParser [
    | text index tok |

    LambdaParser class >> new [
        self error: 'Use new: text'.
    ]

    LambdaParser class >> new: text [
        | r |
        r := super new.
        r init: text.
        ^r
    ]

    LambdaParser class >> parse: text [
        | p |
        p := self new: text.
        ^p parse
    ]

    init: stext [
        text := stext.
        index := 1.
        tok := ''.
    ]

    token [
        | i |
        index := index + (tok size).
        i := index.

        i > text size ifTrue: [
            " Out of bounds "
            tok := nil.
            ^tok
        ].

        (text at: i) isSeparator ifTrue: [
            " Skip it "
            tok := ' '.
            ^self token
        ].

        (text at: i) isAlphaNumeric ifTrue: [
            " Tokenize the whole name or number "
            [(text at: i ifAbsent: [$$]) isAlphaNumeric] whileTrue: [
                i := i + 1.
            ].
            tok := text copyFrom: index to: (i-1).

        ] ifFalse: [
            " All other tokens are of length 1 "
            tok := text copyFrom: i to: i.

        ].

        ^tok
    ]

    parse [
        | r |
        r := self parseApplication.
        r isNil ifTrue: [ self error: 'Failed to parse application' ].
        tok isNil ifFalse: [ self error: 'Unterminated expression' ].
        ^r
    ]

    parseApplication [
        | cur next |
        cur := nil.

        " The top-level parser is for applications "
        [ self token isNil ] whileFalse: [
            next := self parseNonApplication.
            next isNil ifTrue: [
                ^cur
            ].
            cur isNil ifTrue: [
                cur := next.
            ] ifFalse: [
                cur := LambdaApp withRator: cur rand: next.
            ].
        ].

        ^cur
    ]

    parseNonApplication [
        tok = '(' ifTrue: [ ^self parseParenthetical ].
        tok = '^' ifTrue: [ ^self parseAbstraction ].
        (tok at: 1) isAlphaNumeric ifTrue: [ ^self parseVariable ].
        ^nil
    ]

    parseParenthetical [
        | r |
        r := self parseApplication.
        (tok = ')') ifFalse: [
            self error: 'Unterminated parenthetical.'
        ].
        ^r
    ]

    parseAbstraction [
        | var body |

        " We allow a variable form or a variable-less (deBruijn) form, but var
          must be set "
        self token.
        (tok at: 1) isAlphaNumeric ifTrue: [
            var := tok.
            self token.
        ] ifFalse: [
            var := nil.
        ].

        tok = '.' ifFalse: [
            self error: 'Expected .'
        ].

        body := self parseApplication.

        " tok is currently the *next* token, but it should be the last token we
          actually consumed, so stub it "
        tok := ''.

        ^LambdaAbs withVar: var body: body
    ]

    parseVariable [
        | var |
        var := tok.
        (tok at: 1) isDigit ifTrue: [
            " A numeric (deBruijn) variable "
            var := tok asNumber.
        ].
        ^LambdaVar withName: var
    ]
]
