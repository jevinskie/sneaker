What is sneaker?
================
sneaker is an interposing library for Berkeley sockets. With sneaker you will
be able to intercept all socket calls and do all sorts of crazy things with
them, all without modifying existing software. sneaker was created with the
intent of creating polymorphic network enclaves. In such an enclave, IP
addresses and ports are randomized in such a way that only authenticated
systems know the randomized state. Malicious attackers looking into the
network will have difficulty characterizing the services and hosts within
the polymorphic enclave.

Why use Python?
===============
sneaker uses a thin library interposer wrapper written in C to call into
a Python module where the bulk of the logic resides. Python was chosen for
its ease of use and to aid in rapid prototyping. Performance is not yet a
concern but the Python logic could be rewritten in C if needed.

What can sneaker do right now?
==============================
sneaker can currently intercept calls to socket(). Work is underway to
suppor all socket functions.


