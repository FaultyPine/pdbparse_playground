


1) parse PDB -- done
2) parse dumps?
3) analyze running process memory and inspect it with symbols in realtime like D0

1) library & program that starts IPT trace for other process
2) program that reads in an IPT trace and uses https://github.com/intel/libipt/tree/master to decode it
3) symbolicate IPT trace
    3.5) callstacks



Interesting learning


PssCaptureSnapshot
- can be used to capture all the state of a process
- can also walk stuff like pages of the process and thread info and whatnot
- internally uses NtCreateProcessEx with NULL for SectionHandle
    - also needs the target process to be opened (OpenProcess) with the PROCESS_CREATE_PROCESS attribute
    - those two things seem to end up cloning the entire process (threads, mem, stack, etc)
- there also exists RtlCloneUserProcess which interally calls ZwCreateUserProcess, same sorta deal as above in that it internally operates the same that PssCaptureSnapshot does
https://github.com/huntandhackett/process-cloning
- https://engineering.fb.com/2021/04/27/developer-tools/reverse-debugging/
    - meta uses IPT to do "reverse debugging" although idk how fr this is since by the article it doesn't sound like they also capture memory
        - meta's LLDB component for symbolicating IPT traces https://github.com/llvm/llvm-project/tree/main/lldb/source/Plugins/Trace/intel-pt
        https://lldb.llvm.org/use/intel_pt.html


cmake .. -DLLDB_BUILD_INTEL_PT=ON -DLIBIPT_INCLUDE_PATH="C:/Dev/libipt/build/libipt/include" -DLIBIPT_LIBRARY_PATH="C:/Dev\libipt/build/lib" 


- dbghelp.dll is a wrapper on DIA, most people (live++ included) seem to use that over raw DIA which as I've seen, looks pretty nasty syntactically
