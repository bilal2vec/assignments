#![allow(dead_code)]
#![feature(rustc_attrs)]
use std::cell::Cell;
#[rustc_variance]
struct InvariantMut<'a, A: 'a, B: 'a> {
    t: &'a mut (A, B),
}
#[rustc_variance]
use self::Enum::Variant;
#[rustc_variance]
struct InvariantIndirect<A> {
    t: InvariantCell<A>,
}
#[rustc_variance]
struct Covariant<A> {
    t: A,
    u: fn() -> A,
}
#[rustc_variance]
struct Contravariant<A> {
    t: fn(A),
}
#[rustc_variance]
enum Enum<A, B, C> {
    Foo(Covariant<A>),
    Bar(Contravariant<B>),
    Zed(Covariant<C>, Contravariant<C>),
}
pub fn main() {}
