function varargout = difference(varargin)
    %DIFFERENCE \\bried Return all elements of a that do not occur in b, preserving 
    %
    %  {MX} = DIFFERENCE({MX} a, {MX} b)
    %
    %order
    %
    %Doc source: 
    %https://github.com/casadi/casadi/blob/develop/casadi/core/mx.hpp#L854
    %
    %Implementation: 
    %https://github.com/casadi/casadi/blob/develop/casadi/core/mx.hpp#L854-L856
    %
    %
    %
  [varargout{1:nargout}] = casadiMEX(905, varargin{:});
end
