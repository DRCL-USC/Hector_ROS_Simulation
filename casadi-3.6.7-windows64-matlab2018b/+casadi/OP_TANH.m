function v = OP_TANH()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 56);
  end
  v = vInitialized;
end
