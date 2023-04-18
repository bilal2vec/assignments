LambdaExpr extend [
    toDeBruijn [
        ^self toDeBruijn: (Dictionary new)
    ]

    aoe [
        ^self try_aoe
    ]

    nor [
        ^self try_nor
    ]

    eta [
        ^self try_eta
    ]
]

LambdaVar extend [
    toDeBruijn: map [
        | is_bound |

        is_bound := map includesKey: name.

        is_bound ifTrue: [
            ^LambdaVar withName: (map at: name)
        ] ifFalse: [
            ^self dup
        ]
    ]

    try_aoe [
        ^false
    ]

    try_nor [
        ^false
    ]

    try_eta [
        ^false
    ]

    substitute: var_name with: exp [
        var_name = self name ifTrue: [
            ^exp dup
        ].

        ^self dup
    ]
]

LambdaAbs extend [
    toDeBruijn: map [
        | new_map |
        new_map := Dictionary new.
        map keysAndValuesDo: [:k :v | new_map at: k put: v + 1].
        new_map at: var put: 1.

        ^LambdaAbs withVar: nil body: (body toDeBruijn: new_map)
    ]

    try_aoe [
        ^false
    ]

    try_nor [
        | body_nor |

        body_nor := body nor.

        body_nor = false ifTrue: [
            ^false
        ].

        ^LambdaAbs withVar: var body: body_nor dup
    ]

    try_eta [
        | body_eta free_vars |

        body_eta := body eta.

        body_eta ~= false ifTrue: [
            ^LambdaAbs withVar: var body: body_eta dup
        ].

        (body isApp) ifTrue: [
            (body rand) isVar ifTrue: [
                var = ((body rand) name) ifTrue: [
                    free_vars := (body rator) freeVars.

                    (free_vars includesKey: var) ifFalse: [
                        ^(body rator) dup
                    ].
                ].
            ].
        ].

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

            ^LambdaAbs withVar: new_var body: (new_body substitute: var_name with: exp)
        ].
        
        ^LambdaAbs withVar: var body: (body substitute: var_name with: exp)
    ]
]

LambdaApp extend [
    toDeBruijn: map [
        ^LambdaApp withRator: (rator toDeBruijn: map) rand: (rand toDeBruijn: map)
    ]

    try_aoe [
        | rator_aoe rand_aoe |
        
        rator_aoe := rator aoe.
        rator_aoe ~= false ifTrue: [
            ^LambdaApp withRator: (rator_aoe dup) rand: (rand dup)
        ].

        rand_aoe := rand aoe.
        rand_aoe ~= false ifTrue: [
            ^LambdaApp withRator: (rator dup) rand: (rand_aoe dup)
        ].

        rator isAbs ifTrue: [
            | abs_var abs_body |
            
            abs_var := rator var.
            abs_body := rator body.

            ^abs_body substitute: abs_var with: (rand dup)
        ].

        ^false
    ]

    try_nor [
        | rator_nor rand_nor |
        
        rator isAbs ifTrue: [
            | abs_var abs_body |
            
            abs_var := rator var.
            abs_body := rator body.

            ^abs_body substitute: abs_var with: (rand dup)
        ].

        rator_nor := rator nor.
        rator_nor ~= false ifTrue: [
            ^LambdaApp withRator: (rator_nor dup) rand: (rand dup)
        ].

        rand_nor := rand nor.
        rand_nor ~= false ifTrue: [
            ^LambdaApp withRator: (rator dup) rand: (rand_nor dup)
        ].

        ^false
    ]

    try_eta [
        | rator_eta rand_eta |
        
        rator_eta := rator eta.
        rator_eta ~= false ifTrue: [
            ^LambdaApp withRator: (rator_eta dup) rand: (rand dup)
        ].

        rand_eta := rand eta.
        rand_eta ~= false ifTrue: [
            ^LambdaApp withRator: (rator dup) rand: (rand_eta dup)
        ].

        ^false
    ]

    substitute: var_name with: exp [
        ^LambdaApp withRator: (rator substitute: var_name with: exp) rand: (rand substitute: var_name with: exp)
    ]
]


Object subclass: Lambda [
    | exp |
    
    Lambda class >> new: exp [
        | r |
        r := self new.
        r init: exp.
        ^r
    ]

    init: sexp [
        exp := sexp.
    ]

    toDeBruijn [
        exp := exp toDeBruijn.
        ^exp
    ]

    aoe [
        | new_exp |

        new_exp := exp aoe.

        (new_exp = false) ifTrue: [
            exp = nil.
            ^nil
        ].
        
        exp := new_exp.
        ^exp
    ]

    aoe: steps [
        | i new_exp |

        i := 0.

        [i < steps] whileTrue: [
            new_exp := exp aoe.

            (new_exp = false) ifTrue: [
                ^exp
            ].

            exp := new_exp.
            i := i + 1.
        ].

        ^exp
    ]

    nor [
        | new_exp |

        new_exp := exp nor.

        (new_exp = false) ifTrue: [
            exp = nil.
            ^nil
        ].

        exp := new_exp.
        ^exp
    ]

    nor: steps [
        | i new_exp |

        i := 0.

        [i < steps] whileTrue: [
            new_exp := exp nor.

            (new_exp = false) ifTrue: [
                ^exp
            ].

            exp := new_exp.
            i := i + 1.
        ].
    ]

    eta [
        | new_exp |

        new_exp := exp eta.

        (new_exp = false) ifTrue: [
            exp = nil.
            ^nil
        ].

        exp := new_exp.
        ^exp
    ]

    eta: steps [
        | i new_exp |

        i := 0.

        [i < steps] whileTrue: [
            new_exp := exp eta.

            (new_exp = false) ifTrue: [
                ^exp
            ].

            exp := new_exp.
            i := i + 1.
        ].

        ^exp
    ]
]