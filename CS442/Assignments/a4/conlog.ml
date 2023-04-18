open Core
open Core.Poly

(* A substitution is just a string-string pair, but it's common to have substitution lists as well *)
type substitution = string * string

(* A relation of the form functor(args) -> (functor, args) *)
type relation = string * string list

(* A numeric expression. Either a binary expression or a primary, which could
 * be a number or a variable. *)
type expr =
    | Binexp of expr * string * expr
    | Primary of string

(* A comparison of the form Variable Operator Expression ->
 * (Variable, Operator, Expression) *)
type comparison = string * string * expr

(* A single predicate within a conjunction. Either a relation or a comparison. *)
type predicate =
    | Relation of relation
    | Comparison of comparison

(* A Horn clause. The building blocks of our database. *)
type hornClause = relation * predicate list

(* A database is a list of Horn clauses *)
type database = hornClause list


(* Is this string a variable name? *)
let isVarName (s : string) : bool =
    s.[0] >= 'A' && s.[0] <= 'Z'

(* Is this string an atom? *)
let isAtom (s : string) : bool =
    s.[0] >= 'a' && s.[0] <= 'z'

(* Is this string a number? *)
let isNum (s : string) : bool =
    s.[0] >= '0' && s.[0] <= '9'

(* Is this expression just a primary? *)
let isPrimary (exp : expr) : bool =
    match exp with
    | Binexp _ -> false
    | Primary _ -> true

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


(* Parse an argument list within a relation *)
let rec parseArgList (l : string list) : (string list * string list) option =
    match l with
    | [] -> None
    | nm :: rest ->
    if isVarName nm || isAtom nm || isNum nm then
        match rest with
        | "," :: rest -> (
            match parseArgList rest with
            | None -> None
            | Some (al, rest) -> Some (nm :: al, rest)
        )
        | _ -> Some ([nm], rest)
    else
        Some ([], l)

(* Parse (arguments) *)
let parseArgs (l : string list) : (string list * string list) option =
    match l with
    | "(" :: rest -> (
        match parseArgList rest with
        | None -> None
        | Some (al, rest) -> (
            match rest with
            | ")" :: rest -> Some (al, rest)
            | _ -> None
        )
    )
    | _ -> None

(* Parse a relation *)
let parseRelation (l : string list) : (relation * string list) option =
    match l with
    | [] -> None
    | nm :: rest ->
    if isAtom nm then
        match parseArgs rest with
        | None -> None
        | Some (al, rest) -> Some ((nm, al), rest)
    else
        None

(* Parse a parenthetical *)
let rec parseParenthetical (l : string list) : (expr * string list) option =
    match parseExpr l with
    | None -> None
    | Some (exp, rest) ->
    match rest with
    | ")" :: rest -> Some (exp, rest)
    | _ -> None

(* Parse a primary expression *)
and parsePrimary (l : string list) : (expr * string list) option =
    match l with
    | [] -> None
    | "(" :: rest -> parseParenthetical rest
    | nm :: rest ->
        if isVarName nm || isNum nm then
            Some (Primary nm, rest)
        else
            None

(* Parse the "rest" of a multiplicative expression *)
and parseMulExprRight (left : expr) (l : string list) : (expr * string list) option =
    match l with
    | [] -> Some (left, [])
    | "/" :: "/" :: rest -> (
        match parsePrimary rest with
        | None -> None
        | Some (right, rest) -> parseMulExprRight (Binexp (left, "/", right)) rest
    )
    | op :: rest ->
    if op = "*" || op = "/" then
        match parsePrimary rest with
        | None -> None
        | Some (right, rest) -> parseMulExprRight (Binexp (left, op, right)) rest
    else
        Some (left, l)

(* Parse a multiplicative expression *)
and parseMulExpr (l : string list) : (expr * string list) option =
    match parsePrimary l with
    | None -> None
    | Some (left, rest) -> parseMulExprRight left rest

(* Parse the "rest" of an additive expression *)
and parseExprRight (left : expr) (l : string list) : (expr * string list) option =
    match l with
    | [] -> Some (left, [])
    | op :: rest ->
    if op = "+" || op = "-" then
        match parseMulExpr rest with
        | None -> None
        | Some (right, rest) -> parseExprRight (Binexp (left, op, right)) rest
    else
        Some (left, l)

(* Parse an additive expression *)
and parseExpr (l : string list) : (expr * string list) option =
    match parseMulExpr l with
    | None -> None
    | Some (left, rest) -> parseExprRight left rest

(* Parse the rest of a comparison *)
let parseComparison2 (nm : string) (op : string) (l : string list) : (comparison * string list) option =
    match parseExpr l with
    | None -> None
    | Some (exp, rest) -> Some ((nm, op, exp), rest)

(* Parse a comparison *)
let parseComparison (l : string list) : (comparison * string list) option =
    match l with
    | [] -> None
    | nm :: rest ->
    if isVarName nm then
        match rest with
        | ">" :: "=" :: rest -> parseComparison2 nm ">=" rest
        | ">" :: rest -> parseComparison2 nm ">" rest
        | "<" :: rest -> parseComparison2 nm "<" rest
        | "=" :: "<" :: rest -> parseComparison2 nm "=<" rest
        | "=" :: "\\" :: "=" :: rest -> parseComparison2 nm "=\\=" rest
        | "=" :: rest -> parseComparison2 nm "is" rest
        | "is" :: rest -> parseComparison2 nm "is" rest
        | _ -> None
    else
        None

(* Parse a single predicate within a predicate list *)
let parsePredicate (l : string list) : (predicate * string list) option =
    match parseRelation l with
    | Some (rel, rest) -> Some (Relation rel, rest)
    | None ->
    match parseComparison l with
    | Some (c, rest) -> Some (Comparison c, rest)
    | None -> None

(* Parse a predicate list within a Horn clause *)
let rec parsePredicateList (l : string list) : (predicate list * string list) option =
    match parsePredicate l with
    | None -> None
    | Some (predicate, rest) ->
    match rest with
    | "." :: rest -> Some ([predicate], rest)
    | "," :: rest -> (
        match parsePredicateList rest with
        | None -> None
        | Some (pl, rest) -> Some (predicate :: pl, rest)
    )
    | _ -> None

(* Parse a Horn clause *)
let parseHornClause (l : string list) : (hornClause * string list) option =
    match parseRelation l with
    | None -> None
    | Some (rel, rest) ->
    match rest with
    | "." :: rest -> Some ((rel, []), rest)
    | ":" :: "-" :: rest -> (
        match parsePredicateList rest with
        | None -> None
        | Some (pl, rest) -> Some ((rel, pl), rest)
    )
    | _ -> None

(* Parse a list of Horn clauses *)
let rec parseDatabase (l : string list) : database option =
    match l with
    | [] -> Some []
    | _ ->
    match parseHornClause l with
    | None -> None
    | Some (clause, rest) ->
    match parseDatabase rest with
    | None -> None
    | Some cl -> Some (clause :: cl)

(* Parse a predicate list for a query *)
let parsePredicateListStr (s : string) =
    match parsePredicateList (tokenize (String.to_list s)) with
    | None -> None
    | Some (pl, []) -> Some pl
    | _ -> None

(* Parse a whole database from a string *)
let parseDatabaseStr (s : string) = parseDatabase (tokenize (String.to_list s))


(* Add a free variable to a list if it's not already there *)
let freeVarAdd (var : string) (fv : string list) : string list =
    if (List.count ~f:((=) var) fv) > 0 then
        fv
    else
        var :: fv

(* Add all free variables in these arguments *)
let rec freeVarsArgs (args : string list) (fv : string list) : string list =
    match args with
    | [] -> fv
    | nm :: rest ->
    if isVarName nm then
        freeVarsArgs rest (freeVarAdd nm fv)
    else
        freeVarsArgs rest fv

(* Add all free variables in this relation *)
let freeVarsRelation ((_, args) : relation) (fv : string list) : string list =
    freeVarsArgs args fv

(* Add all free variables in an expression *)
let rec freeVarsExpr (exp : expr) (fv : string list) : string list =
    match exp with
    | Binexp (left, _, right) -> (
        let fv2 = freeVarsExpr left fv in
        freeVarsExpr right fv2
    )
    | Primary p -> (
        if isVarName p then
            freeVarAdd p fv
        else
            fv
    )

(* Add all free variables in a comparison *)
let freeVarsComparison ((left, _, right) : comparison) (fv : string list) : string list =
    let fv2 = (
        if isVarName left then
            freeVarAdd left fv
        else
            fv
    ) in
    freeVarsExpr right fv2

(* Add all free variables in this predicate list *)
let rec freeVarsPredicateList (pl : predicate list) (fv : string list) : string list =
    match pl with
    | [] -> fv
    | Relation rel :: rest -> freeVarsPredicateList rest (freeVarsRelation rel fv)
    | Comparison com :: rest -> freeVarsPredicateList rest (freeVarsComparison com fv)

(* Add all free variables in this Horn clause *)
let freeVarsHornClause ((rel, pl) : hornClause) (fv : string list) : string list =
    let fv2 = freeVarsRelation rel fv in
    freeVarsPredicateList pl fv2

(* Find all the free variables in this Horn clause *)
let freeVars (hc : hornClause) : string list =
    freeVarsHornClause hc []


(* Perform a substitution over a single name *)
let rec subName (nm : string) (s : substitution list) : string =
    match s with
    | [] -> nm
    | (from, too) :: rest ->
    if nm = from then
        subName too rest
    else
        subName nm rest

(* Perform a list of substitutions over a list of arguments *)
let rec subArgs (args : string list) (s : substitution list) : string list =
    match args with
    | arg :: rest -> subName arg s :: subArgs rest s
    | [] -> []

(* Perform a list of substitutions over a relation *)
let subRelation ((func, args) : relation) (s : substitution list) : relation =
    (subName func s, subArgs args s)

(* Perform a list of substitutions over an expression *)
let rec subExpr (exp : expr) (s : substitution list) : expr =
    match exp with
    | Binexp (left, op, right) -> Binexp (subExpr left s, op, subExpr right s)
    | Primary p -> Primary (subName p s)

(* Perform a list of substitutions over a comparison *)
let subComparison ((left, op, right) : comparison) (s : substitution list) : comparison =
    (subName left s, op, subExpr right s)

(* Perform a list of substitutions over a list of predicates *)
let rec subPredicateList (pl : predicate list) (s : substitution list) : predicate list =
    match pl with
    | [] -> []
    | Relation rel :: rest -> Relation (subRelation rel s) :: subPredicateList rest s
    | Comparison com :: rest -> Comparison (subComparison com s) :: subPredicateList rest s

(* Perform a list of substitutions over a Horn clause *)
let subHornClause ((rel, pl) : hornClause) (s : substitution list) : hornClause =
    (subRelation rel s, subPredicateList pl s)


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

(* "Freshen" names in this Horn clause, to make sure they can't conflict *)
let freshenHornClause hc =
    let fv = freeVars hc in
    let s = freshenVars fv in
    subHornClause hc s


(* Solve a mathematical expression *)
let rec solve (exp : expr) : int option =
    match exp with
    | Primary p -> (
        if isNum p then
            Some (int_of_string p)
        else
            None
    )
    | Binexp (left, op, right) -> (
        match solve left, op, solve right with
        | Some left, "+", Some right -> Some (left + right)
        | Some left, "-", Some right -> Some (left - right)
        | Some left, "*", Some right -> Some (left * right)
        | Some left, "/", Some right -> Some (left / right)
        | _ -> None
    )


(* Print these relation arguments *)
let rec printArgs a =
    match a with
    | [] -> ()
    | arg :: [] -> printf "%s" arg
    | arg :: al -> (
        printf "%s, " arg;
        printArgs al
    )

(* Print this relation *)
let printRelation ((func, args) : relation) =
    printf "%s(" func;
    printArgs args;
    printf ")"

(* Print this numeric expression *)
let rec printExpr (exp: expr) =
    match exp with
    | Primary p -> printf "%s" p
    | Binexp (left, op, right) -> (
        printf "(";
        printExpr left;
        printf " %s " op;
        printExpr right;
        printf ")"
    )

(* Print this comparison *)
let printComparison ((left, op, right) : comparison) =
    printf "%s %s " left op;
    printExpr right

(* Print this predicate *)
let printPredicate (p : predicate) =
    match p with
    | Relation r -> printRelation r
    | Comparison c -> printComparison c

(* Print this predicate list *)
let rec printPredicateList (pl : predicate list) =
    match pl with
    | [] -> ()
    | p :: [] -> printPredicate p
    | p :: pl -> (
        printPredicate p;
        printf ", ";
        printPredicateList pl
    )

(* Print this Horn clause *)
let printHornClause ((head, body) : hornClause) =
    printRelation head;
    match body with
    | [] -> printf ".\n"
    | _ -> (
        printf " :- ";
        printPredicateList body;
        printf ".\n"
    )

(* Print this database *)
let rec printDatabase (p : database) =
    match p with
    | clause :: cl -> (
        printHornClause clause ;
        printDatabase cl
    )
    | _ -> ()

(* Print this list of substitutions *)
let rec printSubstitutions (ss : substitution list) =
    match ss with
    | [] -> ()
    | (f, t) :: rest ->
    printf "%s -> %s\n" f t;
    printSubstitutions rest
