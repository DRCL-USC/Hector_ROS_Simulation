function varargout = no_grad(varargin)
    %NO_GRAD Stop first derivatives of an expression wrt to all its symbolic 
    %
    %  MX = NO_GRAD(MX expr)
    %
    %variables.
    %
    %\\seealso stop_diff
    %
    %Extra doc: https://github.com/casadi/casadi/wiki/L_25m
    %
    %Doc source: 
    %https://github.com/casadi/casadi/blob/develop/casadi/core/mx.hpp#L832
    %
    %Implementation: 
    %https://github.com/casadi/casadi/blob/develop/casadi/core/mx.hpp#L832-L834
    %
    %
    %
  [varargout{1:nargout}] = casadiMEX(907, varargin{:});
end
