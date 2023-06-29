import ctypes

kernel32 = ctypes.windll.kernel32
process = kernel32.GetCurrentProcess()
kernel32.SetProcessWorkingSetSize(process, -1, -1)

psapi = ctypes.windll.psapi
psapi.EmptyWorkingSet(process)