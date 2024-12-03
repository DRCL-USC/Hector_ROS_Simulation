function v = OP_LIFT()
  persistent vInitialized;
  if isempty(vInitialized)
    vInitialized = casadiMEX(0, 105);
  end
  v = vInitialized;
end
