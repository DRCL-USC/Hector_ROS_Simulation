function v = OP_ATANH()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 59);
  end
  v = vInitialized;
end
