let _unify_primary (lhs : string) (rhs : string) : Conlog.substitution list option =
  if Conlog.isAtom lhs && Conlog.isAtom rhs then (
    if lhs = rhs then Some [] else None
  ) else if Conlog.isNum lhs && Conlog.isNum rhs then (
    if lhs = rhs then Some [] else None
  ) else if Conlog.isVarName lhs then (
    Some [(lhs, rhs)]
  ) else (
    None
  )

let unify_primary (lhs : string) (rhs : string) : Conlog.substitution list option =
  match _unify_primary lhs rhs with
  | None -> _unify_primary rhs lhs
  | Some (sub_list : Conlog.substitution list) -> Some sub_list

let rec unify_args (lhs_args : string list) (rhs_args : string list) (sub_list : Conlog.substitution list) : Conlog.substitution list option =
  match lhs_args, rhs_args with
  | [], [] -> Some sub_list
  | [], _ -> None
  | _, [] -> None
  | (lhs_arg : string) :: [], (rhs_arg : string) :: [] -> (
    let substituted_lhs_arg : string = Conlog.subName lhs_arg sub_list in
    let substituted_rhs_arg : string = Conlog.subName rhs_arg sub_list in
    match unify_primary substituted_lhs_arg substituted_rhs_arg with
    | None -> None
    | Some (sub_list_prime : Conlog.substitution list) -> Some (List.append sub_list sub_list_prime)
  )
  | (lhs_arg : string) :: (lhs_args_prime : string list), (rhs_arg : string) :: (rhs_args_prime : string list) -> (
    let substituted_lhs_arg : string = Conlog.subName lhs_arg sub_list in
    let substituted_rhs_arg : string = Conlog.subName rhs_arg sub_list in
    match unify_primary substituted_lhs_arg substituted_rhs_arg with
    | None -> None
    | Some (sub_list_prime : Conlog.substitution list) -> unify_args lhs_args_prime rhs_args_prime (List.append sub_list sub_list_prime)
  )

let rec _unify_relation (lhs : Conlog.relation) (rhs : Conlog.relation) : Conlog.substitution list option =
  let ((lhs_name : string), (lhs_args : string list)) = lhs in
  let ((rhs_name : string), (rhs_args : string list)) = rhs in
  if lhs_name <> rhs_name then (
    None 
  ) else if List.length lhs_args <> List.length rhs_args then (
    None
  ) else (
    unify_args lhs_args rhs_args []
  )

let rec unify_relation (lhs : Conlog.relation) (rhs : Conlog.relation) : Conlog.substitution list option =
  match _unify_relation lhs rhs with
  | None -> _unify_relation rhs lhs
  | Some (sub_list : Conlog.substitution list) -> Some sub_list


let reduce_logical_expr (lhs: string) (op: string) (rhs: int) : bool =
  match op with
  | ">" -> int_of_string lhs > rhs
  | ">=" -> int_of_string lhs >= rhs
  | "<" -> int_of_string lhs < rhs
  | "=<" -> int_of_string lhs <= rhs
  | "=\\=" -> int_of_string lhs <> rhs
  | _ -> false

let rec get_unifying_horn_clause (db: Conlog.database) (pred: Conlog.relation) : Conlog.hornClause option =
  match db with
  | [] -> None
  | (clause : Conlog.hornClause) :: (tail : Conlog.database) -> (
    let ((head_prime : Conlog.relation), (body_prime : Conlog.predicate list)) = Conlog.freshenHornClause clause in
    let sub : Conlog.substitution list option = unify_relation pred head_prime in
    match sub with
    | None -> get_unifying_horn_clause tail pred
    | Some _ -> Some clause
)

let rec get_satisfying_horn_clause (full_db: Conlog.database) (db : Conlog.database) (pred : Conlog.relation) (query_prime : Conlog.predicate list) : Conlog.substitution list option =
  match db with
  | [] -> None
  | (clause : Conlog.hornClause) :: (tail : Conlog.database) -> (
    let ((head_prime : Conlog.relation), (body_prime : Conlog.predicate list)) = Conlog.freshenHornClause clause in
    let sub : Conlog.substitution list option = unify_relation pred head_prime in
    match sub with
    | None -> get_satisfying_horn_clause full_db tail pred query_prime
    | Some (sub_list : Conlog.substitution list) -> (
      let body_prime_query_prime : Conlog.predicate list = List.append body_prime query_prime in
      let query_prime_prime : Conlog.predicate list = Conlog.subPredicateList body_prime_query_prime sub_list in
      let sub_prime : Conlog.substitution list option = satisfy full_db query_prime_prime in
      match sub_prime with
      | None -> get_satisfying_horn_clause full_db tail pred query_prime
      | Some (sub_prime_list : Conlog.substitution list) -> Some (List.append sub_list sub_prime_list)
    )
  )


and satisfy (db : Conlog.database) (query : Conlog.predicate list) : Conlog.substitution list option = 
  match query with
  | [] -> Some []
  | (pred : Conlog.predicate) :: (query_prime : Conlog.predicate list) ->
    match pred with
    | Conlog.Comparison ((lhs : string), (op : string), (rhs : Conlog.expr)) -> (
      let lhs_is_num : bool = Conlog.isNum lhs in
      let solve_rhs : int option = Conlog.solve rhs in
      match op, lhs_is_num, solve_rhs with
      | "is", _, _ -> (
        let sub : Conlog.substitution list option = match rhs, solve_rhs with
        | Conlog.Primary (rhs : string), _ -> unify_primary lhs rhs
        | Conlog.Binexp(_, _, _), None -> None
        | _, Some (rhs_value : int) -> unify_primary lhs (string_of_int rhs_value)
        in match sub with
        | None -> None
        | Some (sub_list : Conlog.substitution list) -> (
          let substituted_query_prime : Conlog.predicate list = Conlog.subPredicateList query_prime sub_list in
          let satisfy_query_prime : Conlog.substitution list option = satisfy db substituted_query_prime in
          match satisfy_query_prime with
          | Some (satisfy_query_prime_list : Conlog.substitution list) -> Some (List.append sub_list satisfy_query_prime_list)
          | None -> None
        )
      )
      | _, false, _ -> None
      | _, _, None -> None
      | _, _, Some (rhs_value) -> (
        match reduce_logical_expr lhs op rhs_value with
        | true -> satisfy db query_prime
        | false -> None
      )
  )
  | Conlog.Relation ((name: string), (arguments: string list)) -> get_satisfying_horn_clause db db (name, arguments) query_prime

let query (db: Conlog.database) (query: Conlog.predicate list) : Conlog.substitution list option = satisfy db query
