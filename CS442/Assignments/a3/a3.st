HasntkellExp extend [
    noe: dict [
        ^self try_noe: dict
    ]
]

HasntkellVar extend [
    try_noe: dict [
        | is_in_global |

        is_in_global := dict includesKey: self name.

        is_in_global ifTrue: [
            ^(dict at: self name) dup.
        ].

        ^false
    ]

    substitute: var_name with: exp [
        var_name = self name ifTrue: [
            ^exp dup
        ].

        ^self dup
    ]
]

HasntkellAbs extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        | free_vars |

        var_name = self var ifTrue: [
            ^self dup
        ].

        free_vars := exp freeVars.

        (free_vars includesKey: var) ifTrue: [
            | new_var new_body |

            new_var := var , (self hash asString).
            new_body := body substitute: var with: (LambdaVar withName: new_var).

            ^HasntkellAbs withVar: new_var body: (new_body substitute: var_name with: exp)
        ].
        
        ^HasntkellAbs withVar: var body: (body substitute: var_name with: exp)
    ]

]

HasntkellApp extend [
    try_noe: dict [
        | rator_noe |

        rator isError ifTrue: [
            ^HasntkellError new
        ].

        rand isError ifTrue: [
            ^HasntkellError new
        ].

        rator isAbs ifTrue: [
            | abs_var abs_body |

            abs_var := rator var.
            abs_body := rator body.

            ^abs_body substitute: abs_var with: (rand dup)
        ].

        rator isIOAbs ifTrue: [
            ^rator apply: (rand dup).
        ].

        rator_noe := rator noe: dict.
        rator_noe ~= false ifTrue: [
            ^HasntkellApp withRator: (rator_noe dup) rand: (rand dup)
        ].

        ^false
    ]

    substitute: var_name with: exp [
        ^HasntkellApp withRator: (rator substitute: var_name with: exp) rand: (rand substitute: var_name with: exp)
    ]
]

HasntkellBoolLiteral extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

HasntkellTrue extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

HasntkellFalse extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

HasntkellIf extend [
    try_noe: dict [
        | condition_noe |

        condition isBoolLiteral ifTrue: [
            condition value ifTrue: [
                ^thenExp dup
            ].

            ^elseExp dup
        ].

        condition_noe := condition noe: dict.

        condition_noe ~= false ifTrue: [
            ^HasntkellIf withCondition: (condition_noe dup) thenExp: (thenExp dup) elseExp: (elseExp dup)
        ].

        ^false
    ]

    substitute: var_name with: exp [
        ^HasntkellIf withCondition: (condition substitute: var_name with: exp) thenExp: (thenExp substitute: var_name with: exp) elseExp: (elseExp substitute: var_name with: exp)
    ]
]

HasntkellNum extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

HasntkellNumExp extend [
    try_noe: dict [
        | left_noe right_noe |

        left isNum ifTrue: [
            right isNum ifTrue: [
                | value |

                op = '+' ifTrue: [
                    value := left value + right value.
                ].

                op = '-' ifTrue: [
                    value := left value - right value.

                    value < 0 ifTrue: [
                        value := 0.
                    ].
                ].

                op = '*' ifTrue: [
                    value := left value * right value.
                ].

                op = '=' ifTrue: [
                    value := left value = right value.
                    
                    value ifTrue: [
                        ^HasntkellTrue new
                    ].

                    ^HasntkellFalse new
                ].

                ^HasntkellNum withValue: value
            ].
        ].

        left_noe := left noe: dict.
        left_noe ~= false ifTrue: [
            ^HasntkellNumExp withOp: op left: (left_noe dup) right: (right dup)
        ].

        right_noe := right noe: dict.
        right_noe ~= false ifTrue: [
            ^HasntkellNumExp withOp: op left: (left dup) right: (right_noe dup)
        ].

        ^false
    ]

    substitute: var_name with: exp [
        ^HasntkellNumExp withOp: op left: (left substitute: var_name with: exp) right: (right substitute: var_name with: exp)
    ]
]

HasntkellError extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

HasntkellIOAbs extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

HasntkellIO extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

HasntkellReadNum extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

HasntkellWriteNum extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

HasntkellCurryBinding extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

HasntkellBinding extend [
    try_noe: dict [
        ^false
    ]

    substitute: var_name with: exp [
        ^self dup
    ]
]

Object subclass: HasntkellRefLib [
    HasntkellRefLib class >> globals: dict [
        dict at: 'ref' put: (HasntkellIOAbs withConstructor: HasntkellRef).
        dict at: 'get' put: (HasntkellIOAbs withConstructor: HasntkellGet).
        dict at: 'put' put: (HasntkellIOAbs withConstructor: HasntkellCurryPut). 
        ^dict
    ]
]

HasntkellIO subclass: HasntkellRef [
    | exp |

    HasntkellRef class >> new: sexp [
        | r |
        r := super new.
        r init: sexp.
        ^r
    ]

    init: sexp [
        exp := sexp.
    ]

    dup [
        ^HasntkellRef new: exp dup
    ]

    performIO: globals heap: heap [
        | e v l |
        e := Hasntkell new: exp dup withGlobals: globals.
        v := e eval.
        l := v value hash.
        heap at: l put: v.
        ^v
    ]
]

HasntkellIO subclass: HasntkellGet [
    | exp |

    HasntkellGet class >> new: sexp [
        | r |
        r := super new.
        r init: sexp.
        ^r
    ]

    init: sexp [
        exp := sexp.
    ]

    dup [
        ^HasntkellGet new: exp dup
    ]

    performIO: globals heap: heap [
        | e v l |
        e := Hasntkell new: exp dup withGlobals: globals.
        v := e eval.
        l := v value hash.
        ^heap at: l
    ]
]

HasntkellIOAbs subclass: HasntkellCurryPut [
    | left |

    HasntkellCurryPut class >> new: sleft [
        | r |
        r := super new.
        r init: sleft.
        ^r
    ]

    init: sleft [
        left := sleft.
    ]

    dup [
        ^HasntkellCurryPut new: left dup
    ]

    apply: exp [
        ^HasntkellPut withLeft: left dup right: exp dup
    ]
]

HasntkellIO subclass: HasntkellPut [
    | left right |

    HasntkellPut class >> withLeft: sleft right: sright [
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
        ^HasntkellPut withLeft: left dup right: right dup
    ]

    performIO: globals heap: heap [
        | left_e right_e left_v right_v l |
        left_e := Hasntkell new: left dup withGlobals: globals.
        left_v := left_e eval.
        l := left_v value hash.

        right_e := Hasntkell new: right dup withGlobals: globals.
        right_v := right_e eval.

        heap at: l put: right_v.
        ^HasntkellNum withValue: l
    ]
]

Object subclass: Hasntkell [
    | exp dict |

    Hasntkell class >> new: exp withGlobals: dict [
        | r |
        r := self new.
        r init: exp withGlobals: dict.

        ^r
    ]

    init: sexp withGlobals: sdict [
        exp := sexp.
        dict := sdict.
    ]

    eval [
        | new_exp |

        [true] whileTrue: [
            new_exp := exp noe: dict.

            (new_exp = false) ifTrue: [
                ^exp
            ].

            exp := new_exp.
        ].

        ^exp
    ]
]