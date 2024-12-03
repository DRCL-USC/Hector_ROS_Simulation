function v = OP_ASSIGN()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 17);
  end
  v = vInitialized;
end
