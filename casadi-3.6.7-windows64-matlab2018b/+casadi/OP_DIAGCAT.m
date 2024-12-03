function v = OP_DIAGCAT()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 79);
  end
  v = vInitialized;
end
