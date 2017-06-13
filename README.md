# [CubeHash]

This is a C99 implementation of [CubeHash160-16/32-160-h][submission2] and [CubeHash16-16/32-32-h][submission3], Daniel J. Bernstein's two submissions to round 2 of the [NIST hash function competition][sha-3].

It is based on the author's own implementation using EMM intrinsics, which can be found as part of [SUPERCOP] (`crypto_hash/cubehash1632/emmintrin4/`) and its mirror on GitHub ([direct link to folder][emmintrin4]).

[cubehash]: https://cubehash.cr.yp.to/index.html "Introduction to CubeHash"
[submission2]: https://cubehash.cr.yp.to/submission/tweak.pdf "CubeHash parameter tweak: 16 times faster"
[submission3]: https://cubehash.cr.yp.to/submission2/tweak2.pdf "CubeHash parameter tweak: 10 times smaller MAC overhead"
[sha-3]: http://csrc.nist.gov/groups/ST/hash/sha-3/index.html "NIST Computer Security Division - The SHA-3 Cryptographic Hash Algorithm Competition, November 2007 - October 2012"
[supercop]: https://bench.cr.yp.to/supercop.html "SUPERCOP"
[emmintrin4]: https://github.com/floodyberry/supercop/tree/master/crypto_hash/cubehash1632/emmintrin4 "supercop/crypto_hash/cubehash1632/emmintrin4 at master · floodyberry/supercop"

