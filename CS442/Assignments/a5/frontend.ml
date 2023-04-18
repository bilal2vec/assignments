open Core
open Core.Poly
open In_channel

let argv = Sys.get_argv ()
let progIn = read_all (argv.(1))
let prog = Option.value_exn (SIL.parseSIL progIn)
let arg = (
    if Array.length argv > 2 then
        int_of_string argv.(2)
    else
        0
)
let () = A5.run (SIL.VarAssgStmt ("arg", SIL.Num arg) :: prog)
