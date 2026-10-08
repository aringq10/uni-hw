// 1. ------------------ check that all 6 input numbers are different and 1 <= x <= 6
   0x00000000004010f4 <+0>:	    push   %r14
   0x00000000004010f6 <+2>:	    push   %r13
   0x00000000004010f8 <+4>:	    push   %r12
   0x00000000004010fa <+6>:	    push   %rbp
   0x00000000004010fb <+7>:	    push   %rbx
   0x00000000004010fc <+8>:	    sub    $0x50,%rsp
   0x0000000000401100 <+12>:	mov    %rsp,%r13
   0x0000000000401103 <+15>:	mov    %rsp,%rsi
   0x0000000000401106 <+18>:	call   0x40145c <read_six_numbers> // a, b, c, d, e, f
   0x000000000040110b <+23>:	mov    %rsp,%r14                   // r13 = r14 = rbp = rsp
   0x000000000040110e <+26>:	mov    $0x0,%r12d                  // r12 = 0
   0x0000000000401114 <+32>:	mov    %r13,%rbp
   0x0000000000401117 <+35>:	mov    0x0(%r13),%eax
   0x000000000040111b <+39>:	sub    $0x1,%eax
   0x000000000040111e <+42>:	cmp    $0x5,%eax
   0x0000000000401121 <+45>:	jbe    0x401128 <phase_6+52>       // (a - 1) <= 5 ...; unsigned cmp, a >= 1
   0x0000000000401123 <+47>:	call   0x40143a <explode_bomb>
   0x0000000000401128 <+52>:	add    $0x1,%r12d                  // r12 += 1
   0x000000000040112c <+56>:	cmp    $0x6,%r12d
   0x0000000000401130 <+60>:	je     0x401153 <phase_6+95> # false
   0x0000000000401132 <+62>:	mov    %r12d,%ebx                  // ebx = r12
   0x0000000000401135 <+65>:	movslq %ebx,%rax
   0x0000000000401138 <+68>:	mov    (%rsp,%rax,4),%eax # get b, then c...
   0x000000000040113b <+71>:	cmp    %eax,0x0(%rbp)
   0x000000000040113e <+74>:	jne    0x401145 <phase_6+81>       // a != b, a != c
   0x0000000000401140 <+76>:	call   0x40143a <explode_bomb>
   0x0000000000401145 <+81>:	add    $0x1,%ebx
   0x0000000000401148 <+84>:	cmp    $0x5,%ebx
   0x000000000040114b <+87>:	jle    0x401135 <phase_6+65> // 2 <= 5
   0x000000000040114d <+89>:	add    $0x4,%r13 // [r13] = b
   0x0000000000401151 <+93>:	jmp    0x401114 <phase_6+32>
// 2. ------------------ change input num values to f(x) = 7 - x
   0x0000000000401153 <+95>:	lea    0x18(%rsp),%rsi
   0x0000000000401158 <+100>:	mov    %r14,%rax // rsp
   0x000000000040115b <+103>:	mov    $0x7,%ecx
   0x0000000000401160 <+108>:	mov    %ecx,%edx
   0x0000000000401162 <+110>:	sub    (%rax),%edx
   0x0000000000401164 <+112>:	mov    %edx,(%rax) // a = 7 - a, b = 7 - b, c = 7 - c, d = 7 - d, e = 7 - e, f = 7 - f
                                                   // all(x) 1 <= x <= 6
   0x0000000000401166 <+114>:	add    $0x4,%rax
   0x000000000040116a <+118>:	cmp    %rsi,%rax
   0x000000000040116d <+121>:	jne    0x401160 <phase_6+108>
// 3. ------------------ fill up a 48 byte array z of 6 elements of 8 bytes starting at rsp + 32
//                       according to the input numbers:
//                       f(6) = 0x6032d0
//                       f(5) = 0x6032e0
//                       f(4) = 0x6032f0
//                       f(3) = 0x603300
//                       f(2) = 0x603310
//                       f(1) = 0x603320
   0x000000000040116f <+123>:	mov    $0x0,%esi
   0x0000000000401174 <+128>:	jmp    0x401197 <phase_6+163>
   0x0000000000401176 <+130>:	mov    0x8(%rdx),%rdx // rdx = [0x6032d0 + 8] = 0x6032e0; 7 - x = 2 (x is one of the 6 numbers)
                                                      // rdx = [0x6032e0 + 8] = 0x6032f0; 7 - x = 3
                                                      // rdx = [0x6032f0 + 8] = 0x603300; 7 - x = 4
                                                      // rdx = [0x603300 + 8] = 0x603310; 7 - x = 5
                                                      // rdx = [0x603310 + 8] = 0x603320; 7 - x = 6
   0x000000000040117a <+134>:	add    $0x1,%eax
   0x000000000040117d <+137>:	cmp    %ecx,%eax
   0x000000000040117f <+139>:	jne    0x401176 <phase_6+130>

   0x0000000000401181 <+141>:	jmp    0x401188 <phase_6+148>

   0x0000000000401183 <+143>:	mov    $0x6032d0,%edx
   0x0000000000401188 <+148>:	mov    %rdx,0x20(%rsp,%rsi,2) // write 8 bytes at offset 32
   0x000000000040118d <+153>:	add    $0x4,%rsi
   0x0000000000401191 <+157>:	cmp    $0x18,%rsi
   0x0000000000401195 <+161>:	je     0x4011ab <phase_6+183>
   0x0000000000401197 <+163>:	mov    (%rsp,%rsi,1),%ecx // get a, b ...
   0x000000000040119a <+166>:	cmp    $0x1,%ecx
   0x000000000040119d <+169>:	jle    0x401183 <phase_6+143> // a <= 1, b <= 1 ...
   0x000000000040119f <+171>:	mov    $0x1,%eax
   0x00000000004011a4 <+176>:	mov    $0x6032d0,%edx
   0x00000000004011a9 <+181>:	jmp    0x401176 <phase_6+130>
// 4. ------------------ set [z[i] + 8] = z[i + 1] for 0 <= i <= 4, and [z[5] + 8] = 0
   0x00000000004011ab <+183>:	mov    0x20(%rsp),%rbx // get z[0]
   0x00000000004011b0 <+188>:	lea    0x28(%rsp),%rax // get &z[1]
   0x00000000004011b5 <+193>:	lea    0x50(%rsp),%rsi // old rbx address
   0x00000000004011ba <+198>:	mov    %rbx,%rcx
   0x00000000004011bd <+201>:	mov    (%rax),%rdx // get z[1], z[2], z[3]
   0x00000000004011c0 <+204>:	mov    %rdx,0x8(%rcx) // [z[0] + 8] = z[1]
                                                      // [z[1] + 8] = z[2]
                                                      // [z[2] + 8] = z[3]
                                                      // [z[3] + 8] = z[4]
                                                      // [z[4] + 8] = z[5]
   0x00000000004011c4 <+208>:	add    $0x8,%rax
   0x00000000004011c8 <+212>:	cmp    %rsi,%rax
   0x00000000004011cb <+215>:	je     0x4011d2 <phase_6+222>
   0x00000000004011cd <+217>:	mov    %rdx,%rcx
   0x00000000004011d0 <+220>:	jmp    0x4011bd <phase_6+201>

   0x00000000004011d2 <+222>:	movq   $0x0,0x8(%rdx) // [z[5] + 8] = 0
   0x00000000004011da <+230>:	mov    $0x5,%ebp
// 5. ------------------ rbx is z[0] at the start; check that z[0..5] store addresses to consecutively smaller numbers.
//                       lower 4 bytes (sorted desc) and their corresponding input number:
//                       [0x6032f0] = 0x39c; 4
//                       [0x603300] = 0x2b3; 3
//                       [0x603310] = 0x1dd; 2
//                       [0x603320] = 0x1bb; 1
//                       [0x6032d0] = 0x14c; 6
//                       [0x6032e0] = 0x0a8; 5
   0x00000000004011df <+235>:	mov    0x8(%rbx),%rax // get [z[i] + 8]
   0x00000000004011e3 <+239>:	mov    (%rax),%eax    // get [[z[i] + 8]] = [z[i + 1]] (lower 4 bytes only)
   0x00000000004011e5 <+241>:	cmp    %eax,(%rbx)
   0x00000000004011e7 <+243>:	jge    0x4011ee <phase_6+250> // [z[i]] >= [z[i + 1]]
   0x00000000004011e9 <+245>:	call   0x40143a <explode_bomb>
   0x00000000004011ee <+250>:	mov    0x8(%rbx),%rbx // rbx = [z[i] + 8] = z[i + 1]
   0x00000000004011f2 <+254>:	sub    $0x1,%ebp
   0x00000000004011f5 <+257>:	jne    0x4011df <phase_6+235> // ebp != 1
                                                              // repeat 5 times
   0x00000000004011f7 <+259>:	add    $0x50,%rsp
   0x00000000004011fb <+263>:	pop    %rbx
   0x00000000004011fc <+264>:	pop    %rbp
   0x00000000004011fd <+265>:	pop    %r12
   0x00000000004011ff <+267>:	pop    %r13
   0x0000000000401201 <+269>:	pop    %r14
   0x0000000000401203 <+271>:	ret
