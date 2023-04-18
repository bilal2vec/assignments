Object subclass: HasntkellExp [
    isVar [ ^false ]
    isAbs [ ^false ]
    isApp [ ^false ]
    isBoolLiteral [ ^false ]
    isTrue [ ^false ]
    isFalse [ ^false ]
    isIf [ ^false ]
    isNum [ ^false ]
    isNumExp [ ^false ]
    isError [ ^false ]
    isIOAbs [ ^false ]
    isIO [ ^false ]

    isValue [ ^false ]

    reduceWith: block [
        | exp steps next |
        next := self.
        [ next isNil ] whileFalse: [
            exp := next.
            next := block value: exp.
        ].
        ^exp
    ]

    freeVars [
        ^self freeVars: (Dictionary new)
    ]

    freeVars: map [
        ^map
    ]

    printString [
        ^self displayString
    ]
]

HasntkellExp subclass: HasntkellVar [
    | name |

    HasntkellVar class >> withName: name [
        | r |
        r := super new.
        r init: name.
        ^r
    ]

    init: sname [
        name := sname.
    ]

    dup [
        ^HasntkellVar withName: name
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

HasntkellExp subclass: HasntkellAbs [
    | var body |

    HasntkellAbs class >> withVar: var body: body [
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
        ^HasntkellAbs withVar: var body: body dup
    ]

    isAbs [ ^true ]
    isValue [ ^true ]

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

HasntkellExp subclass: HasntkellApp [
    | rator rand |

    HasntkellApp class >> withRator: rator rand: rand [
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
        ^HasntkellApp withRator: rator dup rand: rand dup
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

HasntkellExp subclass: HasntkellBoolLiteral [
    isBoolLiteral [ ^true ]
    isValue [ ^true ]
]

HasntkellBoolLiteral subclass: HasntkellTrue [
    value [ ^true ]
    dup [ ^HasntkellTrue new ]
    isTrue [ ^true ]
    displayString [ ^'true' ]
]

HasntkellBoolLiteral subclass: HasntkellFalse [
    value [ ^false ]
    dup [ ^HasntkellFalse new ]
    isFalse [ ^true ]
    displayString [ ^'false' ]
]

HasntkellExp subclass: HasntkellIf [
    | condition thenExp elseExp |
    HasntkellIf class >> withCondition: scond thenExp: sthene elseExp: selsee [
        | r |
        r := super new.
        r init: scond thenExp: sthene elseExp: selsee.
        ^r
    ]

    init: scond thenExp: sthene elseExp: selsee [
        condition := scond.
        thenExp := sthene.
        elseExp := selsee.
    ]

    dup [
        ^HasntkellIf withCondition: condition dup thenExp: thenExp dup elseExp: elseExp dup
    ]

    isIf [ ^true ]

    condition [ ^condition ]
    thenExp [ ^thenExp ]
    elseExp [ ^elseExp ]

    freeVars: map [
        condition freeVars: map.
        thenExp freeVars: map.
        elseExp freeVars: map.
        ^map
    ]

    freeVars: map [
        condition freeVars: map.
        thenExp freeVars: map.
        elseExp freeVars: map.
        ^map
    ]

    displayString [
        ^'(if ' , condition displayString , ' then ' , thenExp displayString , ' else ' , elseExp displayString , ')'
    ]
]

HasntkellExp subclass: HasntkellNum [
    | value |
    HasntkellNum class >> withValue: value [
        | r |
        r := super new.
        r init: value.
        ^r
    ]

    init: svalue [
        value := svalue.
    ]

    dup [
        ^HasntkellNum withValue: value
    ]

    isNum [ ^true ]
    isValue [ ^true ]

    value [ ^value ]

    displayString [
        ^value asString
    ]
]

HasntkellExp subclass: HasntkellNumExp [
    | op left right |
    HasntkellNumExp class >> withOp: sop left: sleft right: sright [
        | r |
        r := super new.
        r init: sop left: sleft right: sright.
        ^r
    ]

    init: sop left: sleft right: sright [
        op := sop.
        left := sleft.
        right := sright.
    ]

    dup [
        ^HasntkellNumExp withOp: op left: left dup right: right dup
    ]

    isNumExp [ ^true ]

    op [ ^op ]
    left [ ^left ]
    right [ ^right ]

    freeVars: map [
        left freeVars: map.
        right freeVars: map.
        ^map
    ]

    displayString [
        ^'(' , op , ' ' , left displayString , ' ' , right displayString , ')'
    ]
]

HasntkellExp subclass: HasntkellError [
    dup [ ^HasntkellError new ]
    isError [ ^true ]
    isValue [ ^true ]
    displayString [ ^'error' ]
]

HasntkellExp subclass: HasntkellIOAbs [
    | cons |

    HasntkellIOAbs class >> withConstructor: scons [
        | r |
        r := super new.
        r init: scons.
        ^r
    ]

    init: scons [
        cons := scons.
    ]

    dup [
        ^HasntkellIOAbs withConstructor: cons
    ]

    isIOAbs [ ^true ]
    isValue [ ^true ]

    apply: exp [
        ^cons new: exp
    ]

    displayString [
        ^'IOAbs'
    ]
]

HasntkellExp subclass: HasntkellIO [
    isIO [ ^true ]
    isValue [ ^true ]
    displayString [ ^'IO' ]
]

HasntkellIO subclass: HasntkellReadNum [
    dup [
        ^HasntkellReadNum new
    ]

    performIO: globals heap: heap [
        | val |
        val := stdin nextLine asInteger.
        val < 0 ifTrue: [
            val := 0.
        ].
        ^HasntkellNum withValue: val
    ]
]

HasntkellIO subclass: HasntkellWriteNum [
    | exp |

    HasntkellWriteNum class >> new: sexp [
        | r |
        r := super new.
        r init: sexp.
        ^r
    ]

    init: sexp [
        exp := sexp.
    ]

    dup [
        ^HasntkellWriteNum new: exp dup
    ]

    performIO: globals heap: heap [
        | e v |
        e := Hasntkell new: exp dup withGlobals: globals.
        v := e eval.
        v displayNl.
        ^v
    ]
]

HasntkellIOAbs subclass: HasntkellCurryBinding [
    | left |

    HasntkellCurryBinding class >> new: sleft [
        | r |
        r := super new.
        r init: sleft.
        ^r
    ]

    init: sleft [
        left := sleft.
    ]

    dup [
        ^HasntkellCurryBinding new: left dup
    ]

    apply: exp [
        ^HasntkellBinding withLeft: left right: exp
    ]
]

HasntkellIO subclass: HasntkellBinding [
    | left right |

    HasntkellBinding class >> withLeft: sleft right: sright [
        | r |
        r := super new.
        r init: sleft right: sright.
        ^r
    ]

    init: sleft right: sright [
        left := sleft.
        right := sright.
    ]

    dup [
        ^HasntkellBinding withLeft: left dup right: right dup
    ]

    performIO: globals heap: heap [
        | tmp exp e |
        e := Hasntkell new: left withGlobals: globals.
        tmp := e eval.
        tmp := tmp performIO: globals heap: heap.
        exp := HasntkellApp withRator: right rand: tmp.
        e := Hasntkell new: exp withGlobals: globals.
        tmp := e eval.
        ^tmp performIO: globals heap: heap.
    ]
]

Object subclass: HasntkellParser [
    | text index tok |

    HasntkellParser class >> new [
        self error: 'Use new: text'.
    ]

    HasntkellParser class >> new: text [
        | r |
        r := super new.
        r init: text.
        ^r
    ]

    HasntkellParser class >> parse: text [
        | p |
        p := self new: text.
        ^p parse
    ]

    HasntkellParser class >> parseFile: text withGlobals: globals [
        | p |
        p := self new: text.
        ^p parseFile: globals
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

    parseFile: globals [
        | var exp d |
        d := globals.
        [ self token isNil ] whileFalse: [
            " Technically we should check that the variable name is valid, but
              it's pretty harmless to allow any token "
            var := tok.
            self token = '=' ifFalse: [
                self error: 'Expected ='.
            ].
            exp := self parseApplication.
            (tok isNil or: [tok = ';']) ifFalse: [
                self error: 'Expected ;'.
            ].

            d at: var put: exp.
        ].

        ^d
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
                cur := HasntkellApp withRator: cur rand: next.
            ].
        ].

        ^cur
    ]

    parseNonApplication [
        tok = '(' ifTrue: [ ^self parseParenthetical ].
        tok = '^' ifTrue: [ ^self parseAbstraction ].
        tok = 'true' ifTrue: [ ^HasntkellTrue new ].
        tok = 'false' ifTrue: [ ^HasntkellFalse new ].
        tok = 'if' ifTrue: [ ^self parseIf ].
        tok = 'then' ifTrue: [ ^nil ].
        tok = 'else' ifTrue: [ ^nil ].
        (tok at: 1) isDigit ifTrue: [ ^HasntkellNum withValue: tok asNumber ].
        tok = '+' ifTrue: [ ^self parseNumExp ].
        tok = '-' ifTrue: [ ^self parseNumExp ].
        tok = '*' ifTrue: [ ^self parseNumExp ].
        tok = '=' ifTrue: [ ^self parseNumExp ].
        tok = 'error' ifTrue: [ ^HasntkellError new ].
        tok = '[' ifTrue: [ ^self parseTuple ].
        tok = 'let' ifTrue: [ ^self parseLet ].
        tok = 'in' ifTrue: [ ^nil ].
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
            var := ''.
        ].

        tok = '.' ifFalse: [
            self error: 'Expected .'.
        ].

        body := self parseApplication.

        " tok is currently the *next* token, but it should be the last token we
          actually consumed, so stub it "
        tok := ''.

        ^HasntkellAbs withVar: var body: body
    ]

    parseVariable [
        | var |
        var := tok.
        (tok at: 1) isDigit ifTrue: [
            " A numeric (deBruijn) variable "
            var := tok asNumber.
        ].
        ^HasntkellVar withName: var
    ]

    parseIf [
        | cond thenExp elseExp |
        " if (condition) then (exp) else (exp) "
        cond := self parseApplication.
        tok = 'then' ifFalse: [
            self error: 'Expected then'.
        ].
        thenExp := self parseApplication.
        tok = 'else' ifFalse: [
            self error: 'Expected else'.
        ].
        elseExp := self parseApplication.

        tok := ''.
        ^HasntkellIf withCondition: cond thenExp: thenExp elseExp: elseExp
    ]

    parseNumExp [
        | op app l r |
        " We're just going to use parseApplication to get the rest, and then
          extract its rator and rand as left and right "
        op := tok.
        app := self parseApplication.
        app isApp ifFalse: [
            self error: 'Expected multiple expressions.'
        ].

        tok := ''.
        l := app rator.
        r := app rand.
        ^HasntkellNumExp withOp: op left: l right: r
    ]

    parseTuple [
        | r idx el |
        r := HasntkellError new.
        idx := 1.
        [
            self token.
            el := self parseNonApplication.
            el isNil
        ] whileFalse: [
            r := HasntkellIf withCondition:
                (HasntkellNumExp withOp: '=' left: (HasntkellVar withName: '@') right: (HasntkellNum withValue: idx))
            thenExp: el elseExp: r.
            idx := idx + 1.
        ].
        tok = ']' ifFalse: [
            self error: 'Expected ]'.
        ].
        r := HasntkellIf withCondition:
            (HasntkellNumExp withOp: '=' left: (HasntkellVar withName: '@') right: (HasntkellNum withValue: 0))
        thenExp: (HasntkellNum withValue: (idx - 1)) elseExp: r.
        ^HasntkellAbs withVar: '@' body: r
    ]

    parseLet [
        | var val exp |
        " We just rewrite let x = y in z as (^x. z) y "
        var := self token.
        self token = '=' ifFalse: [
            self error: 'Expected ='.
        ].
        val := self parseApplication.
        tok = 'in' ifFalse: [
            self error: 'Expected in'.
        ].
        exp := self parseApplication.
        tok := ''.
        ^HasntkellApp
        withRator: (
            HasntkellAbs withVar: var body: exp
        ) rand: val.
    ]
]

Object subclass: HasntkellPrelude [
    HasntkellPrelude class >> globals [
        | r |
        r := Dictionary new.
        r at: 'readNum' put: HasntkellReadNum new.
        r at: 'writeNum' put: (HasntkellIOAbs withConstructor: HasntkellWriteNum).
        r at: 'bind' put: (HasntkellIOAbs withConstructor: HasntkellCurryBinding).
        r at: 'not' put: (HasntkellParser parse: '^x. if x then false else true').
        r at: 'le' put: (HasntkellParser parse: '^x. ^y. (= (- x y) 0)').
        r at: 'lt' put: (HasntkellParser parse: '^x. ^y. if (= x y) then false else (le x y)').
        r at: 'ge' put: (HasntkellParser parse: '^x. ^y. le y x').
        r at: 'gt' put: (HasntkellParser parse: '^x. ^y. lt y x').
        r at: 'concat' put: (HasntkellParser parse: '
            ^x. ^y. let lenx = (x 0) in ^i.
                if (= i 0)
                then
                    (+ lenx (y 0))
                else if (le i lenx) then
                    (x i)
                else
                    (y (- i lenx))
        ').
        ^r
    ]
]

String extend [
    runHasntkell [
        | globals heap p e h |
        globals := HasntkellPrelude globals.
        globals := HasntkellRefLib globals: globals.
        p := HasntkellParser parseFile: self withGlobals: globals.
        e := p at: 'main'.

        " Evaluate it for an IO, then evaluate the IO "
        h := Hasntkell new: e dup withGlobals: p.
        e := h eval.
        e isIO ifTrue: [
            heap := Dictionary new.
            e := e performIO: p heap: heap.
        ].

        ^e
    ]
]
