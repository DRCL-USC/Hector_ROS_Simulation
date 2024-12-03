function varargout = nlpsol_n_out(varargin)
    %NLPSOL_N_OUT [INTERNAL] 
    %
    %  int = NLPSOL_N_OUT()
    %
    %Number of NLP solver outputs.
    %
    %Extra doc: https://github.com/casadi/casadi/wiki/L_1t3
    %
    %Doc source: 
    %https://github.com/casadi/casadi/blob/develop/casadi/core/nlpsol.hpp#L265
    %
    %Implementation: 
    %https://github.com/casadi/casadi/blob/develop/casadi/core/nlpsol.cpp#L265-L267
    %
    %
    %
  [varargout{1:nargout}] = casadiMEX(833, varargin{:});
end
