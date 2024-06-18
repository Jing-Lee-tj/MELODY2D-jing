function [cpus] = systeminfo()
    [status, cmdout] = system('wmic cpu get NumberOfCores,NumberOfLogicalProcessors');
    parts = strsplit(cmdout);
    cpus = str2double(parts{4});
end