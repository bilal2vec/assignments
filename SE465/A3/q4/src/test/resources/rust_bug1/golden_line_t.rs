#property tests: 45. type E<'a, 'b> = impl Sized;
fn foo<'a: 'b, 'b, 'c>(x: &'static i32, mut y: &'a i32) -> E<'b, 'c> {
    let v = CopyIfEq::<*mut _, *mut _>(&mut { x }, &mut y);
    let a_closure = || {
        22
    };