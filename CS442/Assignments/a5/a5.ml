module Store = Map.Make(String)

type store_value =
  | Num of int
  | Bool of bool
  | Proc of SIL.procedure
  | Arr of int array

let rec reduce_num (expr: SIL.expr) store =
  match expr with
  | Num (n) -> n
  | Var (var_name) -> (
    let found_val = Store.find var_name store in
    match found_val with
    | Num (n) -> n
    | _ -> 0
  )
  | ArrIndexExpr (arr_name, index) -> (
    let reduced_arr_name = match arr_name with
    | Var (var_name) -> var_name
    | _ -> ""
    in
    let found_val = Store.find reduced_arr_name store in
    match found_val with
    | Arr (arr) -> (
      let reduced_index = reduce_num index store in
      Array.get arr reduced_index
    )
    | _ -> 0
  )
  | AddExpr (expr1, expr2) -> (reduce_num expr1 store) + (reduce_num expr2 store)
  | MulExpr (expr1, expr2) -> (reduce_num expr1 store) * (reduce_num expr2 store)
  | NegExpr (expr) -> - (reduce_num expr store)


let rec reduce_bool (expr: SIL.boolExpr) store =
  match expr with
  | True -> true
  | False -> false
  | Not (expr) -> not (reduce_bool expr store)
  | And (expr1, expr2) -> (reduce_bool expr1 store) && (reduce_bool expr2 store)
  | Or (expr1, expr2) -> (reduce_bool expr1 store) || (reduce_bool expr2 store)
  | Gt (expr1, expr2) -> (reduce_num expr1 store) > (reduce_num expr2 store)
  | Lt (expr1, expr2) -> (reduce_num expr1 store) < (reduce_num expr2 store)
  | Eq (expr1, expr2) -> (reduce_num expr1 store) = (reduce_num expr2 store)

let rec build_parameter_argument_list (parameter_list : string list) (argument_list : SIL.expr list) =
  match (parameter_list, argument_list) with
  | ([], []) -> []
  | ([], _) -> []
  | (_, []) -> []
  | ((parameter : string) :: (parameter_list : string list), (argument : SIL.expr) :: (argument_list : SIL.expr list)) -> SIL.VarAssgStmt (parameter, argument) :: (build_parameter_argument_list parameter_list argument_list)

let rec _run (statements : SIL.statement list) store =
  match statements with
  | [] -> ()
  | statement :: statements -> (
    match statement with
    | SIL.Skip -> _run statements store
    | SIL.Block ( block_statements : SIL.statement list ) -> _run (block_statements @ statements) store
    | SIL.WhileStmt ( (condition : SIL.boolExpr) , (body : SIL.statement)) -> (
      let while_body_list = body :: [statement] in 
      let while_body_wrapped = SIL.Block (while_body_list) in
      let while_in_if = SIL.IfStmt (condition, while_body_wrapped, SIL.Skip) in
      _run (while_in_if :: statements) store
    )
    | SIL.IfStmt ( (condition : SIL.boolExpr) , (then_body : SIL.statement) , (else_body : SIL.statement) ) -> (
      match condition with
      | True -> _run (then_body :: statements) store
      | False -> _run (else_body :: statements) store
      | _ -> (
        let reduced_condition = reduce_bool condition store in
        let reduced_condition_wrapped = (match reduced_condition with
          | true -> SIL.True
          | false -> SIL.False)
        in
        _run (SIL.IfStmt (reduced_condition_wrapped, then_body, else_body) :: statements) store
      )
    )
    | SIL.VarAssgStmt ( (var_name : string) , (expr : SIL.expr) ) -> (
      let reduced_expr = match expr with
      | SIL.Var (var_name) -> (
        let found_val = Store.find var_name store in
        match found_val with
        | Num (n) -> Num (n)
        | Proc (procedure : SIL.procedure) -> Proc (procedure)
        | Arr (arr : int array) -> Arr (arr)
        | _ -> Num (0)
      )
      | _ -> Num (reduce_num expr store)
      in
      let store_prime = Store.add var_name reduced_expr store in
      _run statements store_prime
    )
    | SIL.PrintStmt ( (expr : SIL.expr) ) -> (
      let reduced_expr = reduce_num expr store in
      let _ = print_int reduced_expr in
      let _ = print_endline "" in
      _run statements store
    )
    | ProcDecl ( (name : string), (parameter_list : string list), (declaration_list : string list),  (body : SIL.statement list) ) -> (
      let store_prime = Store.add name (Proc (name, parameter_list, declaration_list, body)) store in
      _run statements store_prime
    )
    | CallStmt ( (name : string), (argument_list : SIL.expr list) ) -> (
      let found_val = Store.find name store in
      match found_val with
      | Proc ((name : string), (parameter_list : string list), (declaration_list : string list), (body : SIL.statement list)) -> (
        let ((freshened_parameter_list : string list), (freshened_body : SIL.statement list)) = SIL.freshenProcedure parameter_list declaration_list body in
        let combined_parameter_argument_list : SIL.statement list = build_parameter_argument_list freshened_parameter_list argument_list in 
        _run (SIL.Block (combined_parameter_argument_list @ freshened_body) :: statements) store
      )
      | _ -> ()
    )
    | NewArrStmt ( (name : string), (size : SIL.expr) ) -> (
      let reduced_size = reduce_num size store in
      let arr = Array.make reduced_size 0 in
      let store_prime = Store.add name (Arr (arr)) store in
      _run statements store_prime
    )
    | ArrAssgStmt ( (name : string), (index : SIL.expr), (value : SIL.expr) ) -> (
      let reduced_index = reduce_num index store in
      let reduced_value = reduce_num value store in
      let found_val = Store.find name store in
      match found_val with
      | Arr (arr) -> (
        let _ = Array.set arr reduced_index reduced_value in
        let store_prime = Store.add name (Arr (arr)) store in
        _run statements store_prime
      )
      | _ -> ()
    )
  )

let run (statements : SIL.statement list) =
  let store = Store.empty in 
  _run statements store
