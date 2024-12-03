function varargout = nlpsol_n_in(varargin)
    %NLPSOL_N_IN [INTERNAL] 
    %
    %  int = NLPSOL_N_IN()
    %
    %Number of NLP solver inputs.
    %
    %Extra doc: https://github.com/casadi/casadi/wiki/L_1t2
    %
    %Doc source: 
    %https://github.com/casadi/casadi/blob/develop/casadi/core/nlpsol.hpp#L261
    %
    %Implementation: 
    %https://github.com/casadi/casadi/blob/develop/casadi/core/nlpsol.cpp#L261-L263
    %
    %
    %
  [varargout{1:nargout}] = casadiMEX(832, varargin{:});
end
