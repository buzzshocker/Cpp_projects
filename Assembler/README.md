This is an assembler for a simplified RISC like architecture 

The architecture is called FISC - Four or five instruction set
architecture.

It has the following commands - 

  ADD - Opcode - 00 - add rd rn rm -> rd = rn + rm
  AND - Opcode - 01 - and rd rn rm -> rd = rn ^ rm
  NOT - Opcode - 10 - not rd rn -> rd = ~rn
  BNZ - Opcode - 11 - bnz label - If Z flag = 0, PC = target
  
It has four registers - 
  
  R0 - Opcode - 00
  R1 - Opcode - 01
  R2 - Opcode - 10
  R3 - Opcode - 11
  
Further details on the working and the proper usage are given in the code
