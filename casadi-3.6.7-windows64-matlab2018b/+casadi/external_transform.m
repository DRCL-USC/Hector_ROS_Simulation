function varargout = external_transform(varargin)
    %EXTERNAL_TRANSFORM [INTERNAL] 
    %
    %  Function = EXTERNAL_TRANSFORM(char name, char op, Function f, struct opts)
    %
    %Apply a transformation defined externally.
    %
    %Parameters:
    %-----------
    %
    %name: 
    %Name of the shared library
    %
    %op: 
    %Name of the operation
    %
    %f: 
    % Function to transform
    %
    %opts: 
    %Options
    %
    %::
    %
    %  Extra doc: https://github.com/casadi/casadi/wiki/L_27i 
    %  
    %
    %
    %
    %Doc source: 
    %https://github.com/casadi/casadi/blob/develop/casadi/core/tools.hpp#L45
    %
    %Implementation: 
    %https://github.com/casadi/casadi/blob/develop/casadi/core/tools.cpp#L45-L77
    %
    %
    %
  [varargout{1:nargout}] = casadiMEX(983, varargin{:});
end
