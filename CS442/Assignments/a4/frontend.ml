open Core
open Core.Poly
open In_channel

let dbIn = read_all ((Sys.get_argv ()).(1))
let database = Option.value_exn (Conlog.parseDatabaseStr dbIn)

(* Print these variables with substitutions *)
let rec printVars fv ss =
    match fv with
    | [] -> ()
    | var :: rest ->

    printf "%s = %s\n" var (Conlog.subName var ss);
    printVars rest ss

(* Main entry point *)
let rec main dbg =
    (* Reset the counter before every operation, so our substitutions don't blow up *)
    Conlog.resetCounter ();

    (* Input a line *)
    printf "?- ";
    Out_channel.flush stdout;
    match input_line stdin with
    | None -> ()
    | Some line ->

    (* Check for debug mode *)
    if line = ".dbg" then (
        printf "Debug mode ";
        if dbg then printf "off" else printf "on";
        printf "\n\n";
        main (not dbg)
    ) else

    (* Parse it *)
    match Conlog.parsePredicateListStr line with
    | None -> (
        printf "Parse error!\n\n";
        main dbg
    )
    | Some q ->

    (* Get out the free variables for eventual reporting *)
    let fv = List.rev (Conlog.freeVarsPredicateList q []) in

    (* Debugging *)
    if dbg then (
        printf "Free variables:";
        List.iter ~f:(printf " %s") fv;
        printf "\n\n"
    ) else ();

    (* Perform the query *)
    match A4.query database q with
    | None -> (
        (* No match! *)
        printf "No\n\n";
        main dbg
    )
    | Some ss -> (
        (* Debugging *)
        if dbg then (
            Conlog.printSubstitutions ss;
            printf "\n"
        ) else ();

        (* Matched with substitutions *)
        match fv with
        | [] -> (
            (* No free variables *)
            printf "Yes\n\n";
            main dbg
        )
        | _ -> (
            (* Print substitutions *)
            printVars fv ss;
            printf "\n";
            main dbg
        )
    )

let () = main false
