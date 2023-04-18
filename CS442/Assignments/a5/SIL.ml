open Core
open Core.Poly

(* Substitutions are string pairs *)
type substitution = string * string

(* Non-boolean expressions, which at first are only integer expressions, but
   eventually arrays added *)
type expr =
    (* SIL *)
    | Num of int
    | Var of string
    | AddExpr of (expr * expr)
    | MulExpr of (expr * expr)
    | NegExpr of expr
    (* SIL-PA *)
    | ArrIndexExpr of (expr * expr)

(* Boolean expressions only appear in ifs and whiles, and have several forms *)
type boolExpr =
    | True
    | False
    | Not of boolExpr
    | And of (boolExpr * boolExpr)
    | Or of (boolExpr * boolExpr)
    | Gt of (expr * expr)
    | Lt of (expr * expr)
    | Eq of (expr * expr)

(* Procedures have a name, parameter list, declaration list, and body *)
type procedure = string * string list * string list * statement list

(* Statements have several forms *)
and statement =
    (* SIL *)
    | Skip
    | Block of statement list
    | WhileStmt of (boolExpr * statement)
    | IfStmt of (boolExpr * statement * statement)
    | VarAssgStmt of (string * expr)
    (* SIL-P *)
    | ProcDecl of procedure
    | CallStmt of (string * expr list)
    (* SIL-PA *)
    | NewArrStmt of (string * expr)
    | ArrAssgStmt of (string * expr * expr)
    (* Utility *)
    | PrintStmt of expr

(* A program is a list of statements *)
type program = statement list


(* Is this token an ID-like? *)
let isID (s : string) =
    (s.[0] >= 'A' && s.[0] <= 'Z') || (s.[0] >= 'a' && s.[0] <= 'z')

(* Is this token a variable name? i.e., an ID other than a keyword *)
let isVar (s : string) =
    isID s &&
    not (List.exists ~f:(fun x -> (x = s)) [
        "skip"; "begin"; "end"; "while"; "do"; "if"; "then"; "else"; "true";
        "false"; "not"; "and"; "or"; "procedure"; "array"
    ])

(* Is this token a number? *)
let isNum (s : string) =
    s.[0] >= '0' && s.[0] <= '9'


(* Get an ID token *)
let rec idTok (l : char list) : char list * char list =
    match l with
    | c :: r ->
        if (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') then
            let (tok, rest) = idTok r in
                (c :: tok, rest)
        else
            ([], c :: r)
    | [] -> ([], [])

(* Get a number token *)
let rec numTok (l : char list) : char list * char list =
    match l with
    | c :: r ->
        if (c >= '0' && c <= '9') then
            let (tok, rest) = numTok r in
                (c :: tok, rest)
        else
            ([], c :: r)
    | [] -> ([], [])

(* Skip the remainder of this line (for comments) *)
let rec skipLine (l : char list) : char list =
    match l with
    | [] -> []
    | '\n' :: r -> r
    | _ :: r -> skipLine r

(* Get a single token of any sort *)
let rec token (l : char list) : (char list * char list) option =
    match l with
    | c :: r ->
        if c = '%' then
            token (skipLine r)
        else if Char.is_whitespace c then
            token r
        else if (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') then
            Some (idTok l)
        else if (c >= '0' && c <= '9') then
            Some (numTok l)
        else
            Some ([c], r)
    | [] -> None

(* Tokenize a program *)
let rec tokenize (l : char list) : string list =
    match token l with
    | Some (tok, rest) -> String.of_char_list tok :: tokenize rest
    | None -> []


(* Parse a primary expression *)
let rec parsePrimary (l : string list) : (expr * string list) option =
    match l with
    | v :: rest -> (
        if isNum v then
            Some (Num (int_of_string v), rest)
        else if isVar v then
            Some (Var v, rest)
        else
            None
    )
    | _ -> None

(* Parse an *integer/array* expression parenthetical *)
and parseParenthetical (l : string list) : (expr * string list) option =
    match l with
    | "(" :: rest -> (
        match parseExpr rest with
        | None -> None
        | Some (expr, rest) ->
        match rest with
        | ")" :: rest -> Some (expr, rest)
        | _ -> None
    )
    | _ -> parsePrimary l

(* Parse the rest of an array index expression *)
and parseArrIndexExprRight (left : expr) (l : string list) : (expr * string list) option =
    match l with
    | "[" :: rest -> (
        match parseExpr rest with
        | None -> None
        | Some (right, rest) ->
        match rest with
        | "]" :: rest -> parseArrIndexExprRight (ArrIndexExpr (left, right)) rest
        | _ -> None
    )
    | _ -> Some (left, l)

(* Parse an array index expression *)
and parseArrIndexExpr (l : string list) : (expr * string list) option =
    match parseParenthetical l with
    | None -> None
    | Some (left, rest) -> parseArrIndexExprRight left rest

(* Parse a negation expression *)
and parseNegExpr (l : string list) : (expr * string list) option =
    match l with
    | "-" :: rest -> (
        match parseNegExpr rest with
        | None -> None
        | Some (expr, rest) -> Some (NegExpr expr, rest)
    )
    | _ -> parseArrIndexExpr l

(* Parse the rest of a multiplicative expression *)
and parseMulExprRight (left : expr) (l : string list) : (expr * string list) option =
    match l with
    | "*" :: rest -> (
        match parseNegExpr rest with
        | None -> None
        | Some (right, rest) -> parseMulExprRight (MulExpr (left, right)) rest
    )
    | _ -> Some (left, l)

(* Parse a multiplicative expression *)
and parseMulExpr (l : string list) : (expr * string list) option =
    match parseNegExpr l with
    | None -> None
    | Some (left, rest) -> parseMulExprRight left rest

(* Parse the rest of an additive expression *)
and parseExprRight (left : expr) (l : string list) : (expr * string list) option =
    match l with
    | "+" :: rest -> (
        match parseMulExpr rest with
        | None -> None
        | Some (right, rest) -> parseExprRight (AddExpr (left, right)) rest
    )
    | _ -> Some (left, l)

(* Parse an integer expression, i.e., an additive expression *)
and parseExpr (l : string list) : (expr * string list) option =
    match parseMulExpr l with
    | None -> None
    | Some (left, rest) -> parseExprRight left rest

(* Parse a boolean primary, which is either a literal or a comparison operation *)
let rec parseBoolPrimary (l : string list) : (boolExpr * string list) option =
    match l with
    | "true" :: rest -> Some (True, rest)
    | "false" :: rest -> Some (False, rest)
    | _ ->
    match parseExpr l with
    | None -> None
    | Some (left, rest) ->
    match rest with
    | [] -> None
    | op :: rest ->
    match parseExpr rest with
    | None -> None
    | Some (right, rest) ->
    match op with
    | ">" -> Some (Gt (left, right), rest)
    | "<" -> Some (Lt (left, right), rest)
    | "=" -> Some (Eq (left, right), rest)
    | _ -> None

(* Parse a *boolean* parenthetical. It's important to know whether you're
   parsing a boolean or integer parenthetical. *)
and parseBoolParenthetical (l : string list) : (boolExpr * string list) option =
    match l with
    | "(" :: rest -> (
        match parseBoolExpr rest with
        | None -> parseBoolPrimary l
        | Some (b, rest) ->
        match rest with
        | ")" :: rest -> Some (b, rest)
        | _ -> parseBoolPrimary l
    )
    | _ -> parseBoolPrimary l

(* Parse a not expression *)
and parseNotExpr (l : string list) : (boolExpr * string list) option =
    match l with
    | "not" :: rest -> (
        match parseNotExpr rest with
        | None -> None
        | Some (b, rest) -> Some (Not b, rest)
    )
    | _ -> parseBoolParenthetical l

(* Parse the rest of an and expression *)
and parseAndExprRight (left : boolExpr) (l : string list) : (boolExpr * string list) option =
    match l with
    | "and" :: rest -> (
        match parseNotExpr rest with
        | None -> None
        | Some (right, rest) -> parseAndExprRight (And (left, right)) rest
    )
    | _ -> Some (left, l)

(* Parse an and expression *)
and parseAndExpr (l : string list) : (boolExpr * string list) option =
    match parseNotExpr l with
    | None -> None
    | Some (left, rest) -> parseAndExprRight left rest

(* Parse the rest of an or expression *)
and parseBoolExprRight (left : boolExpr) (l : string list) : (boolExpr * string list) option =
    match l with
    | "or" :: rest -> (
        match parseAndExpr rest with
        | None -> None
        | Some (right, rest) -> parseBoolExprRight (Or (left, right)) rest
    )
    | _ -> Some (left, l)

(* Parse a boolean expression (i.e., an or expression) *)
and parseBoolExpr (l : string list) : (boolExpr * string list) option =
    match parseAndExpr l with
    | None -> None
    | Some (left, rest) -> parseBoolExprRight left rest

(* Parse the rest of an argument list *)
let rec parseArgListRight (left : expr list) (l : string list) : (expr list * string list) option =
    match l with
    | "," :: rest -> (
        match parseExpr rest with
        | None -> None
        | Some (right, rest) -> parseArgListRight (right :: left) rest
    )
    | _ -> Some (left, l)

(* Parse an argument list *)
let parseArgList (l : string list) : (expr list * string list) option =
    match parseExpr l with
    | None -> Some ([], l)
    | Some (left, rest) ->
    match parseArgListRight [left] rest with
    | None -> None
    | Some (al, rest) -> Some (List.rev al, rest)

(* Parse a declaration list *)
let rec parseDeclList (l : string list) : (string list * string list) option =
    match l with
    | "begin" :: _ -> Some ([], l)
    | v :: ";" :: rest -> (
        if not (isVar v) then None else
        match parseDeclList rest with
        | None -> None
        | Some (dl, rest) -> Some (v :: dl, rest)
    )
    | _ -> None

(* Parse the rest of a parameter list *)
let rec parseParamListRight (left : string list) (l : string list) : (string list * string list) option =
    match l with
    | "," :: right :: rest -> (
        if not (isVar right) then None else
        parseParamListRight (right :: left) rest
    )
    | _ -> Some (left, l)

(* Parse a parameter list (list of variables) *)
let parseParamList (l : string list) : (string list * string list) option =
    match l with
    | [] -> Some ([], l)
    | left :: rest ->
    if isVar left then (
        match parseParamListRight [left] rest with
        | None -> None
        | Some (pl, rest) -> Some (List.rev pl, rest)
    ) else (
        Some ([], l)
    )

(* Parse a block *)
let rec parseBlock (l : string list) : (statement * string list) option =
    match l with
    | "begin" :: rest -> (
        match parseStatementList rest with
        | None -> None
        | Some (sl, rest) ->
        match rest with
        | "end" :: rest -> Some (Block sl, rest)
        | _ -> None
    )
    | _ -> None

(* Parse a while loop *)
and parseWhile (l : string list) : (statement * string list) option =
    match l with
    | "while" :: rest -> (
        match parseBoolExpr rest with
        | None -> None
        | Some (cond, rest) ->
        match rest with
        | "do" :: rest -> (
            match parseStatement rest with
            | None -> None
            | Some (body, rest) -> Some (WhileStmt (cond, body), rest)
        )
        | _ -> None
    )
    | _ -> None

(* Parse an if statement *)
and parseIf (l : string list) : (statement * string list) option =
    match l with
    | "if" :: rest -> (
        match parseBoolExpr rest with
        | None -> None
        | Some (cond, rest) ->
        match rest with
        | "then" :: rest -> (
            match parseStatement rest with
            | None -> None
            | Some (thenStmt, rest) ->
            match rest with
            | "else" :: rest -> (
                match parseStatement rest with
                | None -> None
                | Some (elseStmt, rest) ->
                    Some (IfStmt (cond, thenStmt, elseStmt), rest)
            )
            | _ -> None
        )
        | _ -> None
    )
    | _ -> None

(* Parse a procedure declaration statement *)
and parseProcedure (l : string list) : (statement * string list) option =
    match l with
    | "procedure" :: nm :: "(" :: rest -> (
        if not (isVar nm) then None else
        match parseParamList rest with
        | None -> None
        | Some (params, rest) ->
        match rest with
        | ")" :: ";" :: rest -> (
            match parseDeclList rest with
            | None -> None
            | Some (decls, rest) ->
            match rest with
            | "begin" :: rest -> (
                match parseStatementList rest with
                | None -> None
                | Some (body, rest) ->
                match rest with
                | "end" :: rest -> Some (ProcDecl (nm, params, decls, body), rest)
                | _ -> None
            )
            | _ -> None
        )
        | _ -> None
    )
    | _ -> None

(* Parse a write statement *)
and parsePrint (l : string list) : (statement * string list) option =
    match l with
    | "print" :: rest -> (
        match parseExpr rest with
        | None -> None
        | Some (expr, rest) -> Some (PrintStmt expr, rest)
    )
    | _ -> None

(* Parse an array assignment *)
and parseArrAssg (l : string list) : (statement * string list) option =
    match l with
    | target :: "[" :: rest -> (
        if not (isVar target) then None else
        match parseExpr rest with
        | None -> None
        | Some (idx, rest) ->
        match rest with
        | "]" :: ":" :: "=" :: rest -> (
            match parseExpr rest with
            | None -> None
            | Some (expr, rest) -> Some (ArrAssgStmt (target, idx, expr), rest)
        )
        | _ -> None
    )
    | _ -> None

(* Parse a procedure call *)
and parseCall (l : string list) : (statement * string list) option =
    match l with
    | target :: "(" :: rest -> (
        if not (isVar target) then None else
        match parseArgList rest with
        | None -> None
        | Some (args, rest) ->
        match rest with
        | ")" :: rest -> Some (CallStmt (target, args), rest)
        | _ -> None
    )
    | _ -> None

(* Parse our simplified new-array statement *)
and parseNewArray (l : string list) : (statement * string list) option =
    match l with
    | target :: ":" :: "=" :: "array" :: "[" :: rest -> (
        if not (isVar target) then None else
        match parseExpr rest with
        | None -> None
        | Some (sz, rest) ->
        match rest with
        | "]" :: rest -> Some (NewArrStmt (target, sz), rest)
        | _ -> None
    )
    | _ -> None

(* Parse a regular variable assignment *)
and parseVarAssg (l : string list) : (statement * string list) option =
    match l with
    | target :: ":" :: "=" :: rest -> (
        if not (isVar target) then None else
        match parseExpr rest with
        | None -> None
        | Some (expr, rest) -> Some (VarAssgStmt (target, expr), rest)
    )
    | _ -> None

(* Parse an SIL statement *)
and parseStatement (l : string list) : (statement * string list) option =
    match l with
    | "skip" :: rest -> Some (Skip, rest)
    | "begin" :: _ -> parseBlock l
    | "while" :: _ -> parseWhile l
    | "if" :: _ -> parseIf l
    | "procedure" :: _ -> parseProcedure l
    | "print" :: _ -> parsePrint l
    | _ :: "[" :: _ -> parseArrAssg l
    | _ :: "(" :: _ -> parseCall l
    | _ :: ":" :: "=" :: "array" :: "[" :: _ -> parseNewArray l
    | _ :: ":" :: "=" :: _ -> parseVarAssg l
    | _ -> None

(* Parse a statement list *)
and parseStatementList (l : string list) : (statement list * string list) option =
    match parseStatement l with
    | None -> None
    | Some (s1, rest) ->
    match rest with
    | ";" :: rest -> (
        match parseStatementList rest with
        | None -> None
        | Some (sl, rest) -> Some (s1 :: sl, rest)
    )
    | _ -> Some ([s1], rest)

(* Parse an SIL[-P[A]] program *)
let parseSIL (s : string) : program option =
    match parseStatementList (tokenize (String.to_list s)) with
    | Some (sl, []) -> Some sl
    | Some (sl, rest) -> (
        List.iter ~f:(fun el -> printf "%s\n" el) rest;
        None
    )
    | _ -> None


(* Perform a substitution over a single name *)
let rec subName (nm : string) (s : substitution list) : string =
    match s with
    | [] -> nm
    | (from, too) :: rest ->
    if nm = from then
        subName too rest
    else
        subName nm rest

(* Substitute names in this list of vars *)
let rec subVars (vl : string list) (s : substitution list) : string list =
    match vl with
    | [] -> []
    | v :: rest ->
    subName v s :: subVars rest s

(* Substitute this integer/array expressoion *)
let rec subExpr (expr : expr) (s : substitution list) : expr =
    match expr with
    | Num _ -> expr
    | Var v -> Var (subName v s)
    | AddExpr (l, r) -> AddExpr (subExpr l s, subExpr r s)
    | MulExpr (l, r) -> MulExpr (subExpr l s, subExpr r s)
    | NegExpr e -> NegExpr (subExpr e s)
    | ArrIndexExpr (l, r) -> ArrIndexExpr (subExpr l s, subExpr r s)

(* Substitute this boolean expression *)
let rec subBoolExpr (b : boolExpr) (s : substitution list) : boolExpr =
    match b with
    | True -> True
    | False -> False
    | Not b -> Not (subBoolExpr b s)
    | And (l, r) -> And (subBoolExpr l s, subBoolExpr r s)
    | Or (l, r) -> Or (subBoolExpr l s, subBoolExpr r s)
    | Gt (l, r) -> Gt (subExpr l s, subExpr r s)
    | Lt (l, r) -> Lt (subExpr l s, subExpr r s)
    | Eq (l, r) -> Eq (subExpr l s, subExpr r s)

(* Substitute this argument list *)
let rec subArgs (al : expr list) (s : substitution list) : expr list =
    match al with
    | [] -> []
    | a :: rest -> subExpr a s :: subArgs rest s

(* Substitute names in this statement *)
let rec subStatement (st : statement) (s : substitution list) : statement =
    match st with
    | Skip -> Skip
    | Block sl -> Block (subStatementList sl s)
    | WhileStmt (cond, body) ->
        WhileStmt (subBoolExpr cond s, subStatement body s)
    | IfStmt (cond, thenStmt, elseStmt) ->
        IfStmt (subBoolExpr cond s, subStatement thenStmt s, subStatement elseStmt s)
    | VarAssgStmt (target, expr) ->
        VarAssgStmt (subName target s, subExpr expr s)
    | ProcDecl (name, params, decls, body) ->
        ProcDecl (
            subName name s,
            subVars params s,
            subVars decls s,
            subStatementList body s
        )
    | CallStmt (target, args) -> CallStmt (subName target s, subArgs args s)
    | NewArrStmt (target, sz) -> NewArrStmt (subName target s, subExpr sz s)
    | ArrAssgStmt (target, idx, v) ->
        ArrAssgStmt (subName target s, subExpr idx s, subExpr v s)
    | PrintStmt expr -> PrintStmt (subExpr expr s)

(* Substitute names in this statement list *)
and subStatementList (ss : statement list) (s : substitution list) : statement list =
    match ss with
    | [] -> []
    | st :: rest ->
    subStatement st s :: subStatementList rest s


(* Global counter for fresh names *)
let freshNameCounter = ref 0

(* Reset the counter *)
let resetCounter _ =
    freshNameCounter := 0

(* "Freshen" a list of variables *)
let rec freshenVars vl =
    match vl with
    | [] -> []
    | v :: rest ->
    let fc = !freshNameCounter in
    freshNameCounter := !freshNameCounter + 1;
    (v, v ^ "_" ^ string_of_int fc) :: freshenVars rest

(* "Freshen" names in this procedure. Returns the new parameter names and body. *)
let freshenProcedure (params : string list) (decls : string list) (body : statement list) : string list * statement list =
    let s = freshenVars (List.append params decls) in
    subVars params s, subStatementList body s



(* Print an integer/array expression *)
let rec printExpr (e : expr) =
    let printBinary op (l, r) = (
        printf "(";
        printExpr l;
        printf " %s " op;
        printExpr r;
        printf ")"
    ) in

    match e with
    | Num v -> printf "%d" v
    | Var v -> printf "%s" v
    | AddExpr b -> printBinary "+" b
    | MulExpr b -> printBinary "*" b
    | NegExpr e -> (
        printf "(-";
        printExpr e;
        printf ")"
    )
    | ArrIndexExpr (target, idx) -> (
        printf "(";
        printExpr target;
        printf "[";
        printExpr idx;
        printf "])"
    )

(* Print a boolean expression *)
let rec printBoolExpr (b : boolExpr) =
    let printBinary op (l, r) = (
        printf "(";
        printBoolExpr l;
        printf " %s " op;
        printBoolExpr r;
        printf ")"
    ) in
    let printComparison op (l, r) = (
        printf "(";
        printExpr l;
        printf " %s " op;
        printExpr r;
        printf ")"
    ) in

    match b with
    | True -> printf "true"
    | False -> printf "false"
    | Not b -> (
        printf "(not ";
        printBoolExpr b;
        printf ")"
    )
    | And b -> printBinary "and" b
    | Or b -> printBinary "or" b
    | Gt b -> printComparison ">" b
    | Lt b -> printComparison "<" b
    | Eq b -> printComparison "=" b

(* Print an argument list *)
let rec printArgList (al : expr list) =
    match al with
    | a1 :: a2 :: rest -> (
        printExpr a1;
        printf ", ";
        printArgList (a2 :: rest)
    )
    | a1 :: [] -> printExpr a1
    | [] -> ()

(* Print a declaration list *)
let rec printDeclList (dl : string list) =
    match dl with
    | [] -> ()
    | d1 :: rest -> (
        printf "%s;\n" d1;
        printDeclList rest
    )

(* Print a parameter list *)
let rec printParamList (pl : string list) =
    match pl with
    | p1 :: p2 :: rest -> (
        printf "%s, " p1;
        printParamList (p2 :: rest)
    )
    | p1 :: [] -> printf "%s" p1
    | [] -> ()

(* Print a statement *)
let rec printStatement (s : statement) =
    match s with
    | Skip -> printf "skip"
    | Block sl -> (
        printf "begin\n";
        printStatementList sl;
        printf "end"
    )
    | WhileStmt (cond, body) -> (
        printf "while ";
        printBoolExpr cond;
        printf " do ";
        printStatement body
    )
    | IfStmt (cond, thenStmt, elseStmt) -> (
        printf "if ";
        printBoolExpr cond;
        printf " then ";
        printStatement thenStmt;
        printf " else ";
        printStatement elseStmt
    )
    | VarAssgStmt (target, expr) -> (
        printf "%s := " target;
        printExpr expr
    )
    | ProcDecl (nm, params, decls, body) -> (
        printf "procedure %s (" nm;
        printParamList params;
        printf ");\n";
        printDeclList decls;
        printf "begin\n";
        printStatementList body;
        printf "end"
    )
    | PrintStmt expr -> (
        printf "print ";
        printExpr expr
    )
    | CallStmt (target, args) -> (
        printf "%s(" target;
        printArgList args;
        printf ")"
    )
    | NewArrStmt (target, sz) -> (
        printf "%s := array[" target;
        printExpr sz;
        printf ")"
    )
    | ArrAssgStmt (target, idx, expr) -> (
        printf "%s[" target;
        printExpr idx;
        printf "] := ";
        printExpr expr
    )

(* Print a statement list *)
and printStatementList (l : statement list) =
    match l with
    | s :: s2 :: rest -> (
        printStatement s;
        printf ";\n";
        printStatementList (s2 :: rest)
    )
    | s :: [] -> (
        printStatement s;
        printf "\n"
    )
    | [] -> ()
