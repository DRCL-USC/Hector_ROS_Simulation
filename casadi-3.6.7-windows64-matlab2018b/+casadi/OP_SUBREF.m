function v = OP_SUBREF()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 84);
  end
  v = vInitialized;
end
