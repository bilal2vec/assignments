% Factorial
fac(1, 1).
fac(N, M) :- N > 1, Na is N - 1, fac(Na, Ma), M is Ma * N.

% Basic, exponential fibonacci sequence implementation
fib(0, 0).
fib(1, 1).
fib(N, M) :- N > 1, Na is N - 1, Nb is N - 2, fib(Na, Ma), fib(Nb, Mb), M is Ma + Mb.

% Better, dynamic programming implementation, using fib3 to hold previous states
fib2(0, 0).
fib2(1, 1).
fib2(N, M) :- N > 1, fib3(N, P, M).

fib3(0, X, 0).
fib3(1, 0, 1).
fib3(N, P, M) :- N > 1, Na is N - 1, fib3(Na, Pa, P), M is P + Pa.

% Incredibly terrible prime/composite number checker
isprime(X) :- Y is X // 2, isprime2(X, Y).

isprime2(X, 0).
isprime2(X, 1).
isprime2(X, Y) :- Y > 1, R is (X // Y) * Y, R =\= X, Ya is Y - 1, isprime2(X, Ya).

iscomposite(X) :- Y is X // 2, iscomposite2(X, Y).

iscomposite2(X, Y) :- Y > 1, R is (X // Y) * Y, R is X.
iscomposite2(X, Y) :- Y > 1, Ya is Y - 1, iscomposite2(X, Ya).

% Unbelievably terrible prime sequence generator
prime(1, 2).
prime(X, P) :- Xa is X - 1, prime(Xa, Pa), Pb is Pa + 1, primegen(Pb, P).

primegen(X, P) :- iscomposite(X), Pb is X + 1, primegen(Pb, P).
primegen(X, X) :- isprime(X).

% Basic clause that should always fail
cogito(Y) :- X is 1, X > 2.

% Simple cogito ergo sum example
cogito(descartes).
sum(X) :- cogito(X).

% Example 2 from Module 6
p2(X) :- q2(X), r2(X).
q2(X) :- s2(X), t2(X).
s2(a).
s2(b).
s2(c).
t2(b).
t2(c).
r2(c).

% Summation
sumto(1, 1).
sumto(N, R) :- M is N - 1, sumto(M, P), R is P + N.

% Small mammal example from Module 6 (but in our universe, Porcupines are good pets)
smallMammal(cat).
smallMammal(rabbit).
smallMammal(porcupine).
goodPet(X) :- smallMammal(X).

% Person/ownership example from Module 6
person(24601, jean).
book(9564, greatExpectations, dickens).
owns(24601, 9564).
ownsBook(N, T) :- person(X, N), book(Y, T, A), owns(X, Y).

% Employee/supervisor example from Module 6
employee(34, dantes).
employee(5, villefort).
employee(1, napoleon).
employee(34, faria).

supervisor(1, 5).
supervisor(5, 34).
supervisor(5, 27).

supervisorbyname(X, Y) :- employee(N, X), employee(M, Y), supervisor(N, M).

higher(X, Y) :- employee(NX, X), employee(NY, Y), nhigher(NX, NY).

% Because of this clause, higher should never terminate
nhigher(M, N) :- nhigher(P, N), supervisor(M, P).
nhigher(M, N) :- supervisor(M, N).
