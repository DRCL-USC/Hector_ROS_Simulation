function v = OP_SETNONZEROS_PARAM()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 91);
  end
  v = vInitialized;
end
